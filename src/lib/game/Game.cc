
#include "Game.hh"
#include "DataSource.hh"
#include "IInputHandler.hh"
#include "IRenderer.hh"
#include "IUiHandler.hh"
#include "Menu.hh"

#include "GameOverScreenUiHandler.hh"
#include "GameScreenInputHandler.hh"
#include "GameScreenRenderer.hh"
#include "GameScreenUiHandler.hh"
#include "LoginScreenUiHandler.hh"
#include "MapScreenUiHandler.hh"
#include "OutpostScreenUiHandler.hh"

namespace pge {

Game::Game()
  : utils::CoreObject("game")
{
  setService("game");
  initialize();
}

auto Game::getScreen() const noexcept -> Screen
{
  return m_state.screen;
}

void Game::setScreen(const Screen &screen)
{
  m_state.screen = screen;
}

void Game::generateRenderers(int width, int height, SpriteRenderer &spriteRenderer)
{
  auto game = std::make_unique<GameScreenRenderer>(m_views);
  game->loadResources(width, height, spriteRenderer.getTextureHandler());
  m_renderers[Screen::GAME] = std::move(game);
}

void Game::generateInputHandlers()
{
  m_inputHandlers[Screen::GAME] = std::make_unique<GameScreenInputHandler>(m_views);
}

void Game::generateUiHandlers(int width, int height)
{
  auto login = std::make_unique<LoginScreenUiHandler>();
  login->initializeMenus(width, height);
  m_uiHandlers[Screen::LOGIN] = std::move(login);

  auto outpost = std::make_unique<OutpostScreenUiHandler>();
  outpost->initializeMenus(width, height);
  m_uiHandlers[Screen::OUTPOST] = std::move(outpost);

  auto game = std::make_unique<GameScreenUiHandler>(m_views);
  game->initializeMenus(width, height);
  m_uiHandlers[Screen::GAME] = std::move(game);

  auto map = std::make_unique<MapScreenUiHandler>();
  map->initializeMenus(width, height);
  m_uiHandlers[Screen::MAP] = std::move(map);

  auto gameOver = std::make_unique<GameOverScreenUiHandler>();
  gameOver->initializeMenus(width, height);
  m_uiHandlers[Screen::GAMEOVER] = std::move(gameOver);
}

void Game::processUserInput(const controls::State &c, CoordinateFrame &frame)
{
  if (m_state.disabled)
  {
    log("Ignoring action while menu is disabled");
    return;
  }

  const auto inputHandler = m_inputHandlers.find(getScreen());
  if (inputHandler != m_inputHandlers.end())
  {
    inputHandler->second->processUserInput(c, frame);
  }

  menu::InputHandle res{};
  const auto uiHandler = m_uiHandlers.find(getScreen());
  if (uiHandler != m_uiHandlers.end())
  {
    std::vector<ActionShPtr> actions;
    res = uiHandler->second->processUserInput(c, actions);

    for (const auto &action : actions)
    {
      action->apply(*this);
    }
  }

  const auto lClick = (c.buttons[controls::mouse::LEFT] == controls::ButtonState::RELEASED);
  if (lClick && inputHandler != m_inputHandlers.end() && !res.relevant)
  {
    olc::vf2d it;
    olc::vi2d tp = frame.pixelsToTilesAndIntra(olc::vi2d(c.mPosX, c.mPosY), &it);

    inputHandler->second->performAction(tp.x + it.x, tp.y + it.y, c);
  }

  if (c.keys[controls::keys::P])
  {
    togglePause();
  }
  if (c.keys[controls::keys::M])
  {
    std::optional<Screen> nextScreen{};
    switch (getScreen())
    {
      case Screen::GAME:
        nextScreen = {Screen::MAP};
        break;
      case Screen::MAP:
        nextScreen = {Screen::GAME};
        break;
      default:
        break;
    }
    if (nextScreen)
    {
      setScreen(*nextScreen);
    }
  }
}

void Game::render(SpriteRenderer &engine, const RenderState &state, const RenderingPass pass) const
{
  const auto itRenderer = m_renderers.find(getScreen());
  const auto itUi       = m_uiHandlers.find(getScreen());

  switch (pass)
  {
    case RenderingPass::SPRITES:
      break;
    case RenderingPass::DECAL:
      if (itRenderer != m_renderers.end())
      {
        itRenderer->second->render(engine, state, pass);
      }
      break;
    case RenderingPass::UI:
      if (itUi != m_uiHandlers.end())
      {
        itUi->second->render(engine);
      }
      break;
    case RenderingPass::DEBUG:
      if (itRenderer != m_renderers.end())
      {
        itRenderer->second->render(engine, state, pass);
      }
      break;
    default:
      warn("Unknown rendering pass " + str(pass));
      break;
  }
}

void Game::terminate() noexcept
{
  info("Game has been terminated");
  m_state.terminated = true;
}

bool Game::terminated() const noexcept
{
  return m_state.terminated;
}

bool Game::step(float elapsedSeconds)
{
  // When the game is paused it is not over yet.
  if (m_state.paused)
  {
    return true;
  }

  m_coordinator->update(elapsedSeconds);

  const auto it = m_uiHandlers.find(m_state.screen);
  if (it != m_uiHandlers.end())
  {
    it->second->updateUi();
  }

  return true;
}

void Game::togglePause()
{
  if (m_state.paused)
  {
    resume();
  }
  else
  {
    pause();
  }

  enable(!m_state.paused);
}

void Game::pause()
{
  if (m_state.paused)
  {
    return;
  }

  info("Game is now paused");
  m_state.paused = true;
}

void Game::resume()
{
  if (!m_state.paused)
  {
    return;
  }

  info("Game is now resumed");
  m_state.paused = false;
}

void Game::enable(bool enable)
{
  m_state.disabled = !enable;

  if (m_state.disabled)
  {
    log("Disabled game UI", utils::Level::Verbose);
  }
  else
  {
    log("Enabled game UI", utils::Level::Verbose);
  }
}

void Game::initialize()
{
  bsgo::DataSource dataSource;
  const auto playerId     = dataSource.playerId();
  const auto playerShipId = dataSource.playerShipId();

  m_coordinator = std::make_shared<bsgo::Coordinator>();
  dataSource.initialize(*m_coordinator);

  m_views.shipView   = std::make_shared<bsgo::ShipView>(playerId, m_coordinator);
  m_views.systemView = std::make_shared<bsgo::SystemView>(m_coordinator);
  m_views.targetView = std::make_shared<bsgo::TargetView>(playerShipId, m_coordinator);
}

bool Game::TimedMenu::update(bool active) noexcept
{
  // In case the menu should be active.
  if (active)
  {
    if (!wasActive)
    {
      // Make it active if it's the first time that
      // we detect that it should be active.
      date      = utils::now();
      wasActive = true;
      menu->setVisible(true);
    }
    else if (utils::now() > date + utils::toMilliseconds(duration))
    {
      // Deactivate the menu in case it's been active
      // for too long.
      menu->setVisible(false);
    }
    else
    {
      // Update the alpha value in case it's active
      // for not long enough.
      olc::Pixel c = menu->getBackgroundColor();

      float d = utils::diffInMs(date, utils::now()) / duration;
      c.a     = static_cast<uint8_t>(std::clamp((1.0f - d) * pge::alpha::Opaque, 0.0f, 255.0f));
      menu->setBackground(pge::menu::newColoredBackground(c));
    }
  }
  // Or if the menu shouldn't be active anymore and
  // it's the first time we detect that.
  else if (wasActive)
  {
    // Deactivate the menu.
    menu->setVisible(false);
    wasActive = false;
  }

  return menu->visible();
}

} // namespace pge
