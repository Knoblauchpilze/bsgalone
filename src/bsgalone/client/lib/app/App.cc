
#include "App.hh"
#include "AsyncUiCommandQueue.hh"
#include "AsyncUiEventQueue.hh"
#include "DecalRenderer.hh"
#include "EcsCoordinator.hh"
#include "GameBuilder.hh"
#include "GameRenderer.hh"
#include "IUiCommandListener.hh"
#include "IUiEventListener.hh"
#include "LoadingUiHandler.hh"
#include "LoginMessageConsumer.hh"
#include "LoginUiHandler.hh"
#include "LogoutMessageConsumer.hh"
#include "OutpostUiHandler.hh"
#include "OutputUiCommandAdapter.hh"
#include "ServerDataStore.hh"
#include "SignupMessageConsumer.hh"
#include "StatusUiHandler.hh"
#include "SynchronizedUiCommandQueue.hh"
#include "SynchronizedUiEventQueue.hh"
#include "SystemDataMessageConsumer.hh"
#include "UndockMessageConsumer.hh"

namespace bsgalone::client {
App::App(const pge::AppDesc &desc, const NetworkConfig &config)
  : PGEApp(desc)
  , m_config(config)
{}

bool App::onFrame(const float elapsedSeconds)
{
  m_networkClient->processEvents();
  m_uiEventQueue->processEvents();

  if (m_screen == Screen::GAME)
  {
    m_game->update(elapsedSeconds);
  }

  const auto maybeHandler = m_uiHandlers.find(m_screen);
  if (maybeHandler != m_uiHandlers.end())
  {
    maybeHandler->second->updateUi();
  }

  m_uiCommandQueue->processEvents();

  return m_screen == Screen::EXIT;
}

void App::onInputs(const pge::controls::State &controls, pge::CoordinateFrame &frame)
{
  const auto maybeInputHandler = m_inputHandlers.find(m_screen);
  if (maybeInputHandler != m_inputHandlers.end())
  {
    maybeInputHandler->second->processUserInput(controls, frame);
  }

  bool inputRelevantForUi{false};
  const auto maybeUiHandler = m_uiHandlers.find(m_screen);
  if (maybeUiHandler != m_uiHandlers.end())
  {
    ui::UserInputData data{.controls = controls};
    inputRelevantForUi = maybeUiHandler->second->processUserInput(data);
  }

  if (controls.released(pge::controls::mouse::LEFT) && maybeInputHandler != m_inputHandlers.end()
      && !inputRelevantForUi)
  {
    pge::Vec2f it;
    const auto tp = frame.pixelsToTilesAndIntra(pge::Vec2f(controls.mPosX, controls.mPosY), &it);
    maybeInputHandler->second->performAction(tp.x + it.x, tp.y + it.y, controls);
  }
}

void App::loadResources(const pge::Vec2i &screenDims, pge::Renderer &engine)
{
  setLayerTint(Layer::DRAW, semiOpaque(pge::colors::WHITE));

  m_uiCommandQueue = createSynchronizedUiCommandQueue();
  m_uiEventQueue   = createSynchronizedUiEventQueue();

  m_dataStore = std::make_shared<ServerDataStore>();

  auto entityRegistry = std::make_shared<core::EntityRegistry>();
  auto coordinator    = std::make_unique<core::EcsCoordinator>(entityRegistry);

  m_game = GameBuilder()
             .withEntityRegistry(std::move(entityRegistry))
             .withSimulationRunner(std::move(coordinator))
             .build();

  m_networkClient = std::make_shared<GameNetworkClient>();
  initializeIncomingMessageSystem();
  initializeOutgoingMessageSystem();
  initializeInternalMessageSystem();

  generateUiHandlers(screenDims, engine.getTextureHandler());
  generateRenderers(screenDims, engine.getTextureHandler());

  m_networkClient->start(m_config.port);
}

void App::cleanResources()
{
  m_networkClient->stop();

  m_uiEventQueue.reset();

  m_inputHandlers.clear();
  m_uiHandlers.clear();
  m_renderers.clear();

  m_uiCommandQueue.reset();

  m_networkClient.reset();

  m_game.reset();
}

void App::drawDecal(const pge::RenderState &state)
{
  const auto maybeRenderer = m_renderers.find(m_screen);
  if (maybeRenderer == m_renderers.end())
  {
    return;
  }

  maybeRenderer->second->render(state.renderer, state, pge::RenderingPass::DECAL);
}

void App::draw(const pge::RenderState & /*state*/)
{
  // Voluntarily empty, no resources are using the sprites pass.
}

void App::drawUi(const pge::RenderState &state)
{
  const auto maybeHandler = m_uiHandlers.find(m_screen);
  if (maybeHandler == m_uiHandlers.end())
  {
    return;
  }

  maybeHandler->second->render(state.renderer);
}

void App::drawDebug(const pge::RenderState &state, const pge::Vec2f &mouseScreenPos)
{
  const auto maybeRenderer = m_renderers.find(m_screen);
  if (maybeRenderer == m_renderers.end())
  {
    return;
  }

  maybeRenderer->second->render(state.renderer, state, pge::RenderingPass::DEBUG);

  pge::Vec2f it;
  const auto mtp = state.frame.pixelsToTilesAndIntra(mouseScreenPos, &it);

  constexpr auto DEBUG_PIXELS_Y       = 400;
  constexpr auto REASONABLE_PIXEL_GAP = 15;
  pge::Vec2i pos(0, DEBUG_PIXELS_Y);

  state.renderer.drawDebugString(pos,
                                 "Mouse coords      : " + mouseScreenPos.str(),
                                 pge::colors::CYAN);
  pos.y += REASONABLE_PIXEL_GAP;
  state.renderer.drawDebugString(pos, "World cell coords : " + mtp.str(), pge::colors::CYAN);
  pos.y += REASONABLE_PIXEL_GAP;
  state.renderer.drawDebugString(pos, "Intra cell        : " + it.str(), pge::colors::CYAN);
  pos.y += REASONABLE_PIXEL_GAP;
  state.renderer.drawDebugString(pos,
                                 "Screen            : " + str(m_screen),
                                 pge::colors::DARK_GREEN);
}

namespace {

const std::unordered_set<UiCommandType> RELEVANT_UI_COMMAND_TYPES = {
  UiCommandType::EXIT_REQUESTED,
  UiCommandType::LOGIN_REQUESTED,
  UiCommandType::UNDOCK_REQUESTED,
};

class UiCommandListenerProxy : public IUiCommandListener
{
  public:
  explicit UiCommandListenerProxy(App &app)
    : IUiCommandListener()
    , m_app(app)
  {}

  ~UiCommandListenerProxy() override = default;

  bool isEventRelevant(const UiCommandType &type) const override
  {
    return RELEVANT_UI_COMMAND_TYPES.contains(type);
  }

  void onEventReceived(const IUiCommand &command) override
  {
    switch (command.type())
    {
      case UiCommandType::EXIT_REQUESTED:
        m_app.onScreenChanged(Screen::EXIT);
        break;
      case UiCommandType::LOGIN_REQUESTED:
        m_app.onScreenChanged(Screen::LOADING);
        break;
      case UiCommandType::UNDOCK_REQUESTED:
        m_app.onScreenChanged(Screen::LOADING);
        break;
        break;
      default:
        throw std::invalid_argument("Unsupported UI command type " + str(command.type()));
    }
  }

  private:
  App &m_app;
};

const std::unordered_set<UiEventType> RELEVANT_UI_EVENT_TYPES = {
  UiEventType::GAME_READY,
  UiEventType::LOGIN_SUCCEEDED,
  UiEventType::LOGIN_FAILED,
  UiEventType::LOGOUT,
  UiEventType::UNDOCK,
};

class UiEventListenerProxy : public IUiEventListener
{
  public:
  explicit UiEventListenerProxy(App &app)
    : IUiEventListener()
    , m_app(app)
  {}

  ~UiEventListenerProxy() override = default;

  bool isEventRelevant(const UiEventType &type) const override
  {
    return RELEVANT_UI_EVENT_TYPES.contains(type);
  }

  void onEventReceived(const IUiEvent &event) override
  {
    switch (event.type())
    {
      case UiEventType::GAME_READY:
        m_app.onScreenChanged(Screen::GAME);
        break;
      case UiEventType::LOGIN_SUCCEEDED:
        m_app.onScreenChanged(Screen::OUTPOST);
        break;
      case UiEventType::LOGIN_FAILED:
        m_app.onScreenChanged(Screen::LOGIN);
        break;
      case UiEventType::LOGOUT:
        m_app.onScreenChanged(Screen::LOGIN);
        break;
      case UiEventType::UNDOCK:
        m_app.onScreenChanged(Screen::LOADING);
        break;
      default:
        throw std::invalid_argument("Unsupported UI event type " + str(event.type()));
    }
  }

  private:
  App &m_app;
};
} // namespace

void App::initializeIncomingMessageSystem()
{
  m_networkClient->addListener(std::make_unique<SignupMessageConsumer>(m_uiEventQueue));
  m_networkClient->addListener(std::make_unique<LoginMessageConsumer>(m_dataStore, m_uiEventQueue));
  m_networkClient->addListener(
    std::make_unique<LogoutMessageConsumer>(m_dataStore, m_game, m_uiEventQueue));
  m_networkClient->addListener(std::make_unique<UndockMessageConsumer>(m_dataStore, m_uiEventQueue));
  m_networkClient->addListener(std::make_unique<SystemDataMessageConsumer>(m_game, m_uiEventQueue));
}

void App::initializeOutgoingMessageSystem()
{
  m_uiCommandQueue->addListener(std::make_unique<UiCommandListenerProxy>(*this));
  m_uiCommandQueue->addListener(
    std::make_unique<OutputUiCommandAdapter>(m_dataStore, m_networkClient));
}

void App::initializeInternalMessageSystem()
{
  m_uiEventQueue->addListener(std::make_unique<UiEventListenerProxy>(*this));
}

void App::generateUiHandlers(const pge::Vec2i &screenDims,
                             pge::sprites::ITexturePack &texturesLoader)
{
  auto login = std::make_unique<LoginUiHandler>(m_uiEventQueue, m_uiCommandQueue);
  login->initializeMenus(screenDims, texturesLoader);
  m_uiHandlers[Screen::LOGIN] = std::move(login);

  auto loading = std::make_unique<LoadingUiHandler>();
  loading->initializeMenus(screenDims, texturesLoader);
  m_uiHandlers[Screen::LOADING] = std::move(loading);

  auto outpost = std::make_unique<OutpostUiHandler>(m_dataStore, m_uiEventQueue, m_uiCommandQueue);
  outpost->initializeMenus(screenDims, texturesLoader);
  m_uiHandlers[Screen::OUTPOST] = std::move(outpost);

  auto status = std::make_unique<StatusUiHandler>(m_uiCommandQueue);
  status->initializeMenus(screenDims, texturesLoader);
  m_uiHandlers[Screen::GAME] = std::move(status);
}

namespace {
constexpr auto LOGIN_TEXTURE_FILE_PATH   = "assets/login_bg.png";
constexpr auto LOADING_TEXTURE_FILE_PATH = "assets/loading_bg.png";
constexpr auto OUTPOST_TEXTURE_FILE_PATH = "assets/outpost_bg.png";
} // namespace

void App::generateRenderers(const pge::Vec2i &dimensions, pge::sprites::ITexturePack &texturesLoader)
{
  auto login = std::make_unique<DecalRenderer>(LOGIN_TEXTURE_FILE_PATH);
  login->loadResources(dimensions, texturesLoader);
  m_renderers[Screen::LOGIN] = std::move(login);

  auto loading = std::make_unique<DecalRenderer>(LOADING_TEXTURE_FILE_PATH);
  loading->loadResources(dimensions, texturesLoader);
  m_renderers[Screen::LOADING] = std::move(loading);

  auto outpost = std::make_unique<DecalRenderer>(OUTPOST_TEXTURE_FILE_PATH);
  outpost->loadResources(dimensions, texturesLoader);
  m_renderers[Screen::OUTPOST] = std::move(outpost);

  auto game = std::make_unique<GameRenderer>();
  game->loadResources(dimensions, texturesLoader);
  m_renderers[Screen::GAME] = std::move(game);
}

void App::onScreenChanged(const Screen screen)
{
  m_screen = screen;
  info("Switched to screen " + str(screen));
}

} // namespace bsgalone::client
