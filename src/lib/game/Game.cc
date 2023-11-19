
#include "Game.hh"
#include "IInputHandler.hh"
#include "IRenderer.hh"
#include "IUiHandler.hh"
#include "Menu.hh"

#include "GameOverScreenUiHandler.hh"
#include "GameScreenInputHandler.hh"
#include "GameScreenRenderer.hh"
#include "GameScreenUiHandler.hh"
#include "LoginScreenRenderer.hh"
#include "LoginScreenUiHandler.hh"
#include "MapScreenUiHandler.hh"
#include "OutpostScreenRenderer.hh"
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

  auto outpost = std::make_unique<OutpostScreenRenderer>();
  outpost->loadResources(width, height, spriteRenderer.getTextureHandler());
  m_renderers[Screen::OUTPOST] = std::move(outpost);

  auto login = std::make_unique<LoginScreenRenderer>();
  login->loadResources(width, height, spriteRenderer.getTextureHandler());
  m_renderers[Screen::LOGIN] = std::move(login);
}

void Game::generateInputHandlers()
{
  m_inputHandlers[Screen::GAME] = std::make_unique<GameScreenInputHandler>(m_views);
}

void Game::generateUiHandlers(int width, int height)
{
  auto login = std::make_unique<LoginScreenUiHandler>(m_views);
  login->initializeMenus(width, height);
  m_uiHandlers[Screen::LOGIN] = std::move(login);

  auto outpost = std::make_unique<OutpostScreenUiHandler>(m_views);
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

void Game::processUserInput(const controls::State &controls, CoordinateFrame &frame)
{
  const auto inputHandler = m_inputHandlers.find(getScreen());
  if (inputHandler != m_inputHandlers.end())
  {
    inputHandler->second->processUserInput(controls, frame);
  }

  UserInputData uid{.controls = controls};
  bool userInputRelevant{false};
  const auto uiHandler = m_uiHandlers.find(getScreen());
  if (uiHandler != m_uiHandlers.end())
  {
    userInputRelevant = uiHandler->second->processUserInput(uid);

    for (const auto &action : uid.actions)
    {
      action->apply(*this);
    }
  }

  if (controls.released(controls::mouse::LEFT) && inputHandler != m_inputHandlers.end()
      && !userInputRelevant)
  {
    olc::vf2d it;
    olc::vi2d tp = frame.pixelsToTilesAndIntra(olc::vi2d(controls.mPosX, controls.mPosY), &it);

    inputHandler->second->performAction(tp.x + it.x, tp.y + it.y, controls);
  }

  if (controls.keys[controls::keys::M])
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
  m_coordinator->update(elapsedSeconds);

  const auto it = m_uiHandlers.find(m_state.screen);
  if (it != m_uiHandlers.end())
  {
    it->second->updateUi();
  }

  return true;
}

void Game::tryActivateWeapon(const bsgo::Uuid &ship, const int &weaponId)
{
  if (m_views.shipView->isReady())
  {
    m_views.shipView->tryActivateWeapon(ship, weaponId);
  }
}

void Game::tryActivateSlot(const bsgo::Uuid &ship, const int &slotId)
{
  if (m_views.shipView->isReady())
  {
    m_views.shipView->tryActivateSlot(ship, slotId);
  }
}

void Game::login(const bsgo::Uuid &playerDbId)
{
  m_dataSource.setPlayerId(playerDbId);
  m_dataSource.initialize(*m_coordinator);

  const auto playerShipId       = m_dataSource.playerShipId();
  const auto playerShipEntityId = m_dataSource.playerShipEntityId();

  m_views.playerView->setPlayerDbId(playerDbId);
  m_views.playerView->setPlayerShipDbId(playerShipId);
  m_views.shipView->setPlayerShipDbId(playerShipId);
  m_views.shipView->setPlayerShipEntityId(playerShipEntityId);

  setScreen(Screen::OUTPOST);
}

void Game::initialize()
{
  const auto repositories = m_dataSource.repositories();

  m_views.loginView  = std::make_shared<bsgo::LoginView>(m_coordinator, repositories);
  m_views.shipView   = std::make_shared<bsgo::ShipView>(m_coordinator, repositories);
  m_views.systemView = std::make_shared<bsgo::SystemView>(m_coordinator, repositories);
  m_views.playerView = std::make_shared<bsgo::PlayerView>(m_coordinator, repositories);
  m_views.shopView   = std::make_shared<bsgo::ShopView>(m_coordinator, repositories);
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
