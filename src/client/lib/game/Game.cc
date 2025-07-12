
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

Game::Game(const int serverPort,
           const std::optional<std::string> &userName,
           const std::optional<std::string> &password)
  : core::CoreObject("game")
  , m_userName(userName)
  , m_password(password)
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
  return Screen::OUTPOST == current && Screen::GAME == next;
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

  if (m_userName && m_password)
  {
    auto login = std::make_unique<bsgo::LoginMessage>(*m_userName, *m_password);
    login->setClientId(clientId);
    m_outputMessageQueue->pushMessage(std::move(login));
  }
}

void Game::onLogin(const bsgo::Uuid playerDbId)
{
  info("Processing login for " + bsgo::str(playerDbId));
  m_gameSession.onPlayerLoggedIn(playerDbId);

  m_views.playerView->setPlayerDbId(playerDbId);
  m_views.shopView->setPlayerDbId(playerDbId);
  m_views.serverView->setPlayerDbId(playerDbId);

  m_entityMapper.setPlayerDbId(playerDbId);

  setupLoadingScreen(Screen::OUTPOST);
}

void Game::onLoginDataReceived(const bsgo::Uuid playerShipDbId)
{
  info("Received active ship " + bsgo::str(playerShipDbId));
  m_gameSession.onActiveShipChanged(playerShipDbId);
  m_views.shipDbView->setPlayerShipDbId(playerShipDbId);

  m_entityMapper.setPlayerShipDbId(playerShipDbId);
}

void Game::onLogout()
{
  m_gameSession.onPlayerLoggedOut();

  m_coordinator->clear();
  m_entityMapper.clearAll();

  setScreen(Screen::LOGIN);
}

void Game::onActiveShipChanged(const bsgo::Uuid shipDbId)
{
  m_gameSession.onActiveShipChanged(shipDbId);
  m_views.shipDbView->setPlayerShipDbId(shipDbId);

  m_entityMapper.setPlayerShipDbId(shipDbId);

  resetViewsAndUi();
}

void Game::onActiveSystemChanged(const bsgo::Uuid systemDbId)
{
  m_views.shipView->clearJumpSystem();
  m_views.shipDbView->clearJumpSystem();
  m_gameSession.onActiveSystemChanged(systemDbId);

  info("clearing entity mapper");
  m_coordinator->clear();
  m_entityMapper.clearEntities();

  setupLoadingScreen(Screen::GAME);
}

void Game::onShipDocked()
{
  m_entityMapper.clearEntities();
  setScreen(Screen::OUTPOST);
}

void Game::onShipUndocked()
{
  m_coordinator->clear();
  m_entityMapper.clearEntities();

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

void Game::onLoadingStarted(const bsgo::LoadingTransition transition)
{
  m_gameSession.startLoadingTransition(transition);
}

void Game::onLoadingFinished(const bsgo::LoadingTransition transition)
{
  const auto [previousScreen, nextScreen] = m_gameSession.finishLoadingTransition(transition);
  m_state.screen                          = previousScreen;
  setScreen(nextScreen);
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
  createMessageConsumers(*m_inputMessageQueue, m_entityMapper, m_coordinator);

  m_internalMessageQueue->addListener(
    std::make_unique<InternalMessageConsumer>(m_entityMapper, m_coordinator));

  auto messageModule = std::make_unique<GameMessageModule>(*this, m_entityMapper);
  m_inputMessageQueue->addListener(std::move(messageModule));
}

void Game::resetViewsAndUi()
{
  const auto maybePlayerShipEntityId = m_entityMapper.tryGetPlayerShipEntityId();
  m_views.shipView->setPlayerShipEntityId(maybePlayerShipEntityId);

  for (const auto &[_, handler] : m_uiHandlers)
  {
    handler->reset();
  }
}

void Game::setupLoadingScreen(const Screen nextScreen)
{
  m_gameSession.setupLoadingScreen(m_state.screen, nextScreen);
  setScreen(Screen::LOADING);
}

} // namespace pge
