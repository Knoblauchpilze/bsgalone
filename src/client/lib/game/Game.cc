
#include "Game.hh"
#include "ClientMessageConsumerUtils.hh"
#include "GameMessageModule.hh"
#include "IInputHandler.hh"
#include "IRenderer.hh"
#include "IUiHandler.hh"
#include "InternalMessageConsumer.hh"
#include "NetworkSystem.hh"
#include "SynchronizedMessageQueue.hh"

#include "GameScreenInputHandler.hh"
#include "GameScreenRenderer.hh"
#include "GameScreenUiHandler.hh"
#include "LoadingScreenRenderer.hh"
#include "LoadingScreenUiHandler.hh"
#include "LoginScreenRenderer.hh"
#include "LoginScreenUiHandler.hh"
#include "MapScreenRenderer.hh"
#include "MapScreenUiHandler.hh"
#include "OutpostScreenRenderer.hh"
#include "OutpostScreenUiHandler.hh"

namespace pge {

Game::Game(const int serverPort)
  : core::CoreObject("game")
{
  setService("game");
  initialize(serverPort);
}

Game::~Game()
{
  m_inputMessageQueue.reset();
  m_outputMessageQueue.reset();
  m_networkContext->stop();
}

auto Game::getScreen() const noexcept -> Screen
{
  return m_state.screen;
}

namespace {
bool shouldUpdateCoordinatorAfterScreenTransition(const Screen &current, const Screen &next)
{
  return Screen::LOADING == current && Screen::GAME == next;
}
} // namespace

void Game::setScreen(const Screen &screen)
{
  const auto fullReset = shouldUpdateCoordinatorAfterScreenTransition(m_state.screen, screen);
  if (fullReset)
  {
    info("Resetting game data");
    resetViewsAndUi();
  }
  else
  {
    const auto ui = m_uiHandlers.find(screen);
    if (ui != m_uiHandlers.cend())
    {
      ui->second->reset();
    }
  }

  m_state.screen = screen;
  if (m_state.dead && Screen::OUTPOST == m_state.screen)
  {
    m_state.dead = false;
  }
}

void Game::generateRenderers(int width, int height, Renderer &engine)
{
  auto login = std::make_unique<LoginScreenRenderer>();
  login->loadResources(width, height, engine.getTextureHandler());
  m_renderers[Screen::LOGIN] = std::move(login);

  auto loading = std::make_unique<LoadingScreenRenderer>();
  loading->loadResources(width, height, engine.getTextureHandler());
  m_renderers[Screen::LOADING] = std::move(loading);

  auto outpost = std::make_unique<OutpostScreenRenderer>();
  outpost->loadResources(width, height, engine.getTextureHandler());
  m_renderers[Screen::OUTPOST] = std::move(outpost);

  auto game = std::make_unique<GameScreenRenderer>(m_views);
  game->loadResources(width, height, engine.getTextureHandler());
  m_renderers[Screen::GAME] = std::move(game);

  auto map = std::make_unique<MapScreenRenderer>();
  map->loadResources(width, height, engine.getTextureHandler());
  m_renderers[Screen::MAP] = std::move(map);
}

void Game::generateInputHandlers()
{
  m_inputHandlers[Screen::GAME] = std::make_unique<GameScreenInputHandler>(m_views);
}

void Game::generateUiHandlers(int width, int height, Renderer &engine)
{
  m_uiHandlers[Screen::LOGIN]   = std::make_unique<LoginScreenUiHandler>(m_views);
  m_uiHandlers[Screen::LOADING] = std::make_unique<LoadingScreenUiHandler>(m_views);
  m_uiHandlers[Screen::OUTPOST] = std::make_unique<OutpostScreenUiHandler>(m_views);
  m_uiHandlers[Screen::GAME]    = std::make_unique<GameScreenUiHandler>(m_views);
  m_uiHandlers[Screen::MAP]     = std::make_unique<MapScreenUiHandler>(m_views);

  for (const auto &[_, handler] : m_uiHandlers)
  {
    handler->initializeMenus(width, height, engine.getTextureHandler());
    handler->connectToMessageQueue(*m_inputMessageQueue);
  }
}

namespace {
auto applyInputToGame(const controls::State &controls,
                      const Screen currentScreen,
                      CoordinateFrame &frame,
                      const std::unordered_map<Screen, IInputHandlerPtr> &inputHandlers)
  -> std::optional<IInputHandler *>
{
  const auto maybeInputHandler = inputHandlers.find(currentScreen);
  if (maybeInputHandler == inputHandlers.end())
  {
    return {};
  }

  maybeInputHandler->second->processUserInput(controls, frame);

  return maybeInputHandler->second.get();
}

bool applyInputToUi(const controls::State &controls,
                    const Screen currentScreen,
                    Game &g,
                    const std::unordered_map<Screen, IUiHandlerPtr> &uiHandlers)
{
  UserInputData uid{.controls = controls};
  bool userInputRelevant{false};
  const auto uiHandler = uiHandlers.find(currentScreen);
  if (uiHandler != uiHandlers.end())
  {
    userInputRelevant = uiHandler->second->processUserInput(uid);

    for (const auto &action : uid.actions)
    {
      action(g);
    }
  }

  return userInputRelevant;
}

auto applyInputToScreen(const controls::State &controls, const Screen currentScreen)
  -> std::optional<Screen>
{
  if (!controls.released(controls::keys::M))
  {
    return {};
  }

  switch (currentScreen)
  {
    case Screen::GAME:
      return Screen::MAP;
    case Screen::MAP:
      return Screen::GAME;
    default:
      return {};
  }
}
} // namespace

void Game::processUserInput(const controls::State &controls, CoordinateFrame &frame)
{
  if (m_state.dead)
  {
    applyInputToUi(controls, getScreen(), *this, m_uiHandlers);
    return;
  }

  const auto maybeInputHandler  = applyInputToGame(controls, getScreen(), frame, m_inputHandlers);
  const auto inputRelevantForUi = applyInputToUi(controls, getScreen(), *this, m_uiHandlers);

  if (controls.released(controls::mouse::LEFT) && maybeInputHandler.has_value()
      && !inputRelevantForUi)
  {
    Vec2f it;
    const auto tp = frame.pixelsToTilesAndIntra(Vec2f(controls.mPosX, controls.mPosY), &it);
    (*maybeInputHandler)->performAction(tp.x + it.x, tp.y + it.y, controls);
  }

  const auto nextScreen = applyInputToScreen(controls, getScreen());
  if (nextScreen)
  {
    setScreen(*nextScreen);
  }
}

void Game::render(const RenderState &state, const RenderingPass pass) const
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
        itRenderer->second->render(state.renderer, state, pass);
      }
      break;
    case RenderingPass::UI:
      if (itUi != m_uiHandlers.end())
      {
        itUi->second->render(state.renderer);
      }
      break;
    case RenderingPass::DEBUG:
      if (itRenderer != m_renderers.end())
      {
        itRenderer->second->render(state.renderer, state, pass);
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

namespace {
bool shouldUpdateCoordinatorInScreen(const Screen &screen)
{
  return Screen::GAME == screen || Screen::MAP == screen;
}
} // namespace

bool Game::step(float elapsedSeconds)
{
  // Process messages first and then update the coordinator so that
  // the systems have a chance to react to messages before sending
  // everything to the UI.
  m_inputMessageQueue->processMessages();
  m_internalMessageQueue->processMessages();

  if (shouldUpdateCoordinatorInScreen(m_state.screen))
  {
    m_coordinator->update(elapsedSeconds);
  }

  const auto it = m_uiHandlers.find(m_state.screen);
  if (it != m_uiHandlers.end())
  {
    it->second->updateUi();
  }

  return true;
}

void Game::onConnectedToServer(const bsgo::Uuid clientId)
{
  info("Received client id " + bsgo::str(clientId) + " from server");
  m_outputMessageQueue->setClientId(clientId);

// #define START_AT_LOGIN
#ifndef START_AT_LOGIN
  // auto login = std::make_unique<bsgo::LoginMessage>("toast", "aze");
  auto login = std::make_unique<bsgo::LoginMessage>("colo", "aze");
  login->setClientId(clientId);
  m_outputMessageQueue->pushMessage(std::move(login));
#endif
}

void Game::onLogin(const bsgo::Uuid playerDbId)
{
  info("processing login for " + bsgo::str(playerDbId));
  m_gameSession.playerDbId = playerDbId;
  setupLoadingScreen(Screen::OUTPOST);
}

void Game::onLogout()
{
  if (!m_gameSession.playerDbId)
  {
    error("Failed to log out", "Not logged in");
  }

  info("Player " + bsgo::str(*m_gameSession.playerDbId) + " logged out");
  setScreen(Screen::LOGIN);
  m_gameSession.playerDbId.reset();
  m_gameSession.systemDbId.reset();
}

void Game::onActiveShipChanged()
{
  resetViewsAndUi();
}

void Game::onActiveSystemChanged()
{
  m_views.shipView->clearJumpSystem();
  m_views.shipDbView->clearJumpSystem();
  m_dataSource.clearSystemDbId();
  // TODO: We should set the screen to loading
  resetViewsAndUi();

  setupLoadingScreen(Screen::GAME);
}

void Game::onShipDocked()
{
  setScreen(Screen::OUTPOST);
}

void Game::onShipUndocked()
{
  setupLoadingScreen(Screen::GAME);
}

void Game::onPlayerKilled()
{
  info("Player got killed");
  m_state.dead = true;
  if (m_state.screen != Screen::GAME)
  {
    setScreen(Screen::GAME);
  }
}

void Game::onLoadingStarted(const bsgo::Uuid systemDbId,
                            const std::optional<bsgo::Uuid> maybePlayerDbId)
{
  if (m_state.screen != Screen::LOADING)
  {
    error("Unexpected loading started event", "Not in loading screen");
  }
  debug("Starting loading transition to " + str(*m_gameSession.nextScreen));

  // TODO: Should not be necessary except for the login

  if (*m_gameSession.previousScreen == Screen::LOGIN && !maybePlayerDbId)
  {
    error("Unexpected start of loading process", "No player ID provided");
  }
  if (!m_gameSession.playerDbId || *m_gameSession.playerDbId != *maybePlayerDbId)
  {
    error("Unexpected start of loading process", "Player ID mismatch");
  }

  info("starting loading process for system " + bsgo::str(systemDbId) + " and player "
       + bsgo::str(*maybePlayerDbId));

  m_gameSession.systemDbId = systemDbId;
  m_dataSource.setPlayerDbId(*maybePlayerDbId);

  m_coordinator->clear();
  m_entityMapper.clear();

  m_entityMapper.setPlayerDbId(*maybePlayerDbId);
}

void Game::onLoadingFinished()
{
  if (m_state.screen != Screen::LOADING)
  {
    error("Unexpected loading finished event", "Not in loading screen");
  }
  if (!m_gameSession.nextScreen || !m_gameSession.previousScreen)
  {
    error("Unexpected loading finished event", "No next or previous screen defined");
  }

  const auto previousScreen = *m_gameSession.previousScreen;
  const auto nextScreen     = *m_gameSession.nextScreen;

  debug("Ending loading transition from " + str(previousScreen) + " to " + str(nextScreen));

  m_gameSession.previousScreen.reset();
  m_gameSession.nextScreen.reset();

  m_state.screen = nextScreen;

  m_dataSource.initialize(*m_coordinator, m_entityMapper);
  resetViewsAndUi();
}

void Game::initialize(const int serverPort)
{
  auto connection = std::make_unique<ClientConnection>(serverPort, *m_networkContext);

  m_inputMessageQueue = connection->createInputMessageQueue();
  // Not strictly necessary as the internal messages should only be produced
  // synchronously by the Coordinator but also does not hurt.
  m_internalMessageQueue = std::make_unique<bsgo::SynchronizedMessageQueue>(
    "synchronized-message-queue-for-internal");
  m_outputMessageQueue = std::make_unique<ClientMessageQueue>(std::move(connection));

  const auto repositories = m_dataSource.repositories();
  bsgo::SystemsConfig sConfig{.internalMessageQueue = m_internalMessageQueue.get(),
                              .outputMessageQueue   = m_outputMessageQueue.get(),
                              .ignoredSystems       = {bsgo::SystemType::LOOT,
                                                       bsgo::SystemType::REMOVAL,
                                                       bsgo::SystemType::STATUS,
                                                       bsgo::SystemType::HEALTH,
                                                       bsgo::SystemType::POWER}};
  m_coordinator = std::make_shared<bsgo::Coordinator>(std::move(sConfig));

  bsgo::ViewsConfig vConfig{.repositories         = repositories,
                            .coordinator          = m_coordinator,
                            .internalMessageQueue = m_internalMessageQueue.get(),
                            .outputMessageQueue   = m_outputMessageQueue.get()};
  m_views = bsgo::createViews(vConfig, m_entityMapper);

  initializeMessageSystem();

  m_networkContext->start();
}

void Game::initializeMessageSystem()
{
  createMessageConsumers(m_dataSource.repositories(),
                         *m_inputMessageQueue,
                         m_entityMapper,
                         m_coordinator);

  m_internalMessageQueue->addListener(
    std::make_unique<InternalMessageConsumer>(m_entityMapper, m_coordinator));

  auto messageModule = std::make_unique<GameMessageModule>(*this, m_entityMapper);
  m_inputMessageQueue->addListener(std::move(messageModule));
}

void Game::resetViewsAndUi()
{
  if (!m_gameSession.playerDbId)
  {
    error("Failed to reset views and UI", "Expected to have a player defined");
  }

  m_views.playerView->setPlayerDbId(*m_gameSession.playerDbId);
  m_views.shopView->setPlayerDbId(*m_gameSession.playerDbId);
  m_views.serverView->setPlayerDbId(*m_gameSession.playerDbId);

  auto maybePlayerShipDbId = m_entityMapper.tryGetPlayerShipDbId();
  if (!maybePlayerShipDbId)
  {
    maybePlayerShipDbId = m_dataSource.tryGetPlayerShipDbId();
  }
  m_views.shipDbView->setPlayerShipDbId(maybePlayerShipDbId);

  const auto maybePlayerShipEntityId = m_entityMapper.tryGetPlayerShipEntityId();
  m_views.shipView->setPlayerShipEntityId(maybePlayerShipEntityId);

  for (const auto &[_, handler] : m_uiHandlers)
  {
    handler->reset();
  }
}

void Game::setupLoadingScreen(const Screen nextScreen)
{
  if (m_state.screen == Screen::LOADING)
  {
    error("Unexpected loading screen transition", "Already in loading screen");
  }
  if (m_gameSession.nextScreen)
  {
    error("Unexpected loading screen transition", "No next screen defined");
  }

  m_gameSession.previousScreen = m_state.screen;
  m_gameSession.nextScreen     = nextScreen;
  setScreen(Screen::LOADING);
}

} // namespace pge
