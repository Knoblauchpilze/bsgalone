
#include "Game.hh"
#include "GameOverScreenHandler.hh"
#include "GameScreenHandler.hh"
#include "LoginScreenHandler.hh"
#include "MapScreenHandler.hh"
#include "Menu.hh"
#include "OutpostScreenHandler.hh"

namespace pge {

Game::Game()
  : utils::CoreObject("game")
{
  setService("game");
}

auto Game::generateHandlers(int width, int height, SpriteRenderer &spriteRenderer)
  -> std::unordered_map<Screen, IScreenHandlerPtr>
{
  std::unordered_map<Screen, IScreenHandlerPtr> out;

  bsgo::Views views;
  views.shipView        = std::make_shared<bsgo::ShipView>();
  views.systemView      = std::make_shared<bsgo::SystemView>();
  auto &texturesHandler = spriteRenderer.getTextureHandler();

  auto login = std::make_unique<LoginScreenHandler>();
  login->loadResources(width, height, texturesHandler);
  out[Screen::LOGIN] = std::move(login);

  auto game = std::make_unique<GameScreenHandler>(views);
  game->loadResources(width, height, texturesHandler);
  out[Screen::GAME] = std::move(game);

  auto map = std::make_unique<MapScreenHandler>();
  map->loadResources(width, height, texturesHandler);
  out[Screen::MAP] = std::move(map);

  auto outpost = std::make_unique<OutpostScreenHandler>();
  outpost->loadResources(width, height, texturesHandler);
  out[Screen::OUTPOST] = std::move(outpost);

  auto gameOver = std::make_unique<GameOverScreenHandler>();
  gameOver->loadResources(width, height, texturesHandler);
  out[Screen::GAMEOVER] = std::move(gameOver);

  for (const auto &[screen, handler] : out)
  {
    m_handlers[screen] = handler.get();
  }

  return out;
}

void Game::performAction(float x, float y, const controls::State &state)
{
  // Only handle actions when the game is not disabled.
  if (m_state.disabled)
  {
    log("Ignoring action while menu is disabled");
    return;
  }

  const auto it = m_handlers.find(m_state.screen);
  if (it == m_handlers.end())
  {
    return;
  }

  it->second->performAction(x, y, state);
}

bool Game::step(float /*tDelta*/)
{
  // When the game is paused it is not over yet.
  if (m_state.paused)
  {
    return true;
  }

  updateUI();

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

void Game::updateUI()
{
  const auto it = m_handlers.find(m_state.screen);
  if (it == m_handlers.end())
  {
    return;
  }

  it->second->updateUi();
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
