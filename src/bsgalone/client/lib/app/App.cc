
#include "App.hh"
#include "AsyncUiCommandQueue.hh"
#include "AsyncUiEventQueue.hh"
#include "DecalRenderer.hh"
#include "IUiCommandListener.hh"
#include "IUiEventListener.hh"
#include "LoadingUiHandler.hh"
#include "LoginMessageConsumer.hh"
#include "LoginUiHandler.hh"
#include "LogoutMessageConsumer.hh"
#include "OutputUiCommandAdapter.hh"
#include "ServerDataStore.hh"
#include "SignupMessageConsumer.hh"
#include "StatusUiHandler.hh"
#include "SynchronizedUiCommandQueue.hh"
#include "SynchronizedUiEventQueue.hh"

namespace bsgalone::client {

App::App(const pge::AppDesc &desc, const NetworkConfig &config)
  : PGEApp(desc)
  , m_config(config)
{}

bool App::onFrame(const float /*elapsedSeconds*/)
{
  m_networkClient->processEvents();

  const auto maybeHandler = m_uiHandlers.find(m_screen);
  if (maybeHandler != m_uiHandlers.end())
  {
    maybeHandler->second->updateUi();
  }

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

  m_uiCommandQueue = createAsyncUiCommandQueue(createSynchronizedUiCommandQueue());
  m_uiEventQueue   = createAsyncUiEventQueue(createSynchronizedUiEventQueue());

  m_dataStore = std::make_shared<ServerDataStore>();

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

void App::draw(const pge::RenderState & /*state*/) {}

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
class UiCommandListenerProxy : public IUiCommandListener
{
  public:
  UiCommandListenerProxy(App &app)
    : IUiCommandListener()
    , m_app(app)
  {}

  ~UiCommandListenerProxy() override = default;

  bool isEventRelevant(const UiCommandType &type) const override
  {
    return type == UiCommandType::EXIT_REQUESTED;
  }

  void onEventReceived(const IUiCommand & /*event*/) override
  {
    m_app.onScreenChanged(Screen::EXIT);
  }

  private:
  App &m_app;
};

const std::unordered_set<UiEventType> RELEVANT_UI_EVENT_TYPES = {
  UiEventType::LOGIN_SUCCEEDED,
  UiEventType::LOGOUT,
};

class UiEventListenerProxy : public IUiEventListener
{
  public:
  UiEventListenerProxy(App &app)
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
      case UiEventType::LOGIN_SUCCEEDED:
        m_app.onScreenChanged(Screen::GAME);
        break;
      case UiEventType::LOGOUT:
        m_app.onScreenChanged(Screen::LOGIN);
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
  m_networkClient->addListener(std::make_unique<LogoutMessageConsumer>(m_dataStore, m_uiEventQueue));
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

void App::generateUiHandlers(const pge::Vec2i &screenDims, pge::sprites::TexturePack &texturesLoader)
{
  auto login = std::make_unique<LoginUiHandler>(m_uiEventQueue, m_uiCommandQueue);
  login->initializeMenus(screenDims, texturesLoader);
  m_uiHandlers[Screen::LOGIN] = std::move(login);

  auto loading = std::make_unique<LoadingUiHandler>();
  loading->initializeMenus(screenDims, texturesLoader);
  m_uiHandlers[Screen::LOADING] = std::move(loading);

  auto status = std::make_unique<StatusUiHandler>(m_uiCommandQueue);
  status->initializeMenus(screenDims, texturesLoader);
  m_uiHandlers[Screen::GAME] = std::move(status);
}

namespace {
constexpr auto LOGIN_TEXTURE_FILE_PATH   = "assets/login_bg.png";
constexpr auto LOADING_TEXTURE_FILE_PATH = "assets/loading_bg.png";
constexpr auto GAME_TEXTURE_FILE_PATH    = "assets/game_bg.png";
} // namespace

void App::generateRenderers(const pge::Vec2i &dimensions, pge::sprites::TexturePack &texturesLoader)
{
  auto login = std::make_unique<DecalRenderer>(LOGIN_TEXTURE_FILE_PATH);
  login->loadResources(dimensions, texturesLoader);
  m_renderers[Screen::LOGIN] = std::move(login);

  auto loading = std::make_unique<DecalRenderer>(LOADING_TEXTURE_FILE_PATH);
  loading->loadResources(dimensions, texturesLoader);
  m_renderers[Screen::LOADING] = std::move(loading);

  auto game = std::make_unique<DecalRenderer>(GAME_TEXTURE_FILE_PATH);
  game->loadResources(dimensions, texturesLoader);
  m_renderers[Screen::GAME] = std::move(game);
}

void App::onScreenChanged(const Screen screen)
{
  m_screen = screen;
}

} // namespace bsgalone::client
