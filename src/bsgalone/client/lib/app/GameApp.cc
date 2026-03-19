
#include "GameApp.hh"
#include "DecalScreenRenderer.hh"
#include "LoginScreenRenderer.hh"
#include "LoginUiHandler.hh"
#include "OutputUiCommandAdapter.hh"

namespace bsgalone::client {

GameApp::GameApp(const pge::AppDesc &desc, ServerConfig config)
  : pge::PGEApp(desc)
  , m_serverPort(config.port)
  , m_networkClient(std::make_shared<GameNetworkClient>(config.autoConnect))
{}

bool GameApp::onFrame(const float /*elapsedSeconds*/)
{
  m_networkClient->processEvents();

  const auto maybeUiHandler = m_uiHandlers.find(m_screen);
  if (maybeUiHandler != m_uiHandlers.end())
  {
    maybeUiHandler->second->updateUi();
  }

  m_uiCommandQueue->processEvents();

  return m_screen == Screen::EXIT;
}

void GameApp::onInputs(const pge::controls::State &controls, pge::CoordinateFrame & /*frame*/)
{
  const auto maybeUiHandler = m_uiHandlers.find(m_screen);
  if (maybeUiHandler == m_uiHandlers.end())
  {
    return;
  }

  ui::UserInputData input{.controls = controls};
  maybeUiHandler->second->processUserInput(input);

  if (!input.actions.empty())
  {
    error("Received " + std::to_string(input.actions.size()) + " unsupported game action(s)");
  }
}

namespace {
constexpr auto LOGIN_TEXTURE_FILE_PATH   = "assets/login_bg.png";
constexpr auto LOADING_TEXTURE_FILE_PATH = "assets/loading_screen.png";
} // namespace

void GameApp::loadResources(const pge::Vec2i &screenDims, pge::Renderer &engine)
{
  setLayerTint(Layer::DRAW, pge::semiOpaque(pge::colors::WHITE));

  generateRenderers(screenDims, engine);
  generateUiHandlers(screenDims, engine);
  initializeMessageSystem();

  m_networkClient->start(m_serverPort);
}

void GameApp::cleanResources()
{
  m_networkClient->stop();

  m_renderers.clear();
  m_uiHandlers.clear();

  m_uiCommandQueue.reset();

  m_networkClient.reset();
}

void GameApp::drawDecal(const pge::RenderState &state)
{
  const auto maybeRenderer = m_renderers.find(m_screen);
  if (maybeRenderer != m_renderers.end())
  {
    maybeRenderer->second->render(state.renderer, state, pge::RenderingPass::DECAL);
  }
}

void GameApp::draw(const pge::RenderState & /*state*/) {}

void GameApp::drawUi(const pge::RenderState &state)
{
  const auto maybeUiHandler = m_uiHandlers.find(m_screen);
  if (maybeUiHandler != m_uiHandlers.end())
  {
    maybeUiHandler->second->render(state.renderer);
  }
}

void GameApp::drawDebug(const pge::RenderState &state, const pge::Vec2f &mouseScreenPos)
{
  const auto maybeRenderer = m_renderers.find(m_screen);
  if (maybeRenderer != m_renderers.end())
  {
    maybeRenderer->second->render(state.renderer, state, pge::RenderingPass::DECAL);
  }

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

void GameApp::generateRenderers(const pge::Vec2i &screenDims, pge::Renderer &engine)
{
  auto login = std::make_unique<DecalScreenRenderer>(LOGIN_TEXTURE_FILE_PATH);
  login->loadResources(screenDims.x, screenDims.y, engine.getTextureHandler());
  m_renderers[Screen::LOGIN] = std::move(login);

  auto loading = std::make_unique<DecalScreenRenderer>(LOADING_TEXTURE_FILE_PATH);
  loading->loadResources(screenDims.x, screenDims.y, engine.getTextureHandler());
  m_renderers[Screen::LOADING] = std::move(loading);
}

void GameApp::generateUiHandlers(const pge::Vec2i &screenDims, pge::Renderer &engine)
{
  auto loginUi = std::make_unique<LoginUiHandler>(m_uiCommandQueue);
  loginUi->initializeMenus(screenDims.x, screenDims.y, engine.getTextureHandler());
  m_uiHandlers[Screen::LOGIN] = std::move(loginUi);
}

void GameApp::initializeMessageSystem()
{
  m_uiCommandQueue->addListener(std::make_unique<OutputUiCommandAdapter>(m_networkClient));
}

} // namespace bsgalone::client
