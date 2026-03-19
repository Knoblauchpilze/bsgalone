
#include "GameApp.hh"
#include "DecalScreenRenderer.hh"
#include "LoginScreenRenderer.hh"

namespace bsgalone::client {

GameApp::GameApp(const pge::AppDesc &desc, ServerConfig config)
  : pge::PGEApp(desc)
  , m_serverPort(config.port)
  , m_networkClient(std::make_shared<GameNetworkClient>(config.autoConnect))
{}

bool GameApp::onFrame(const float /*elapsedSeconds*/)
{
  m_networkClient->processEvents();

  return m_screen == Screen::EXIT;
}

void GameApp::onInputs(const pge::controls::State & /*controls*/, pge::CoordinateFrame & /*frame*/)
{}

namespace {
constexpr auto LOGIN_TEXTURE_FILE_PATH   = "assets/login_bg.png";
constexpr auto LOADING_TEXTURE_FILE_PATH = "assets/loading_screen.png";
} // namespace

void GameApp::loadResources(const pge::Vec2i &screenDims, pge::Renderer &engine)
{
  setLayerTint(Layer::DRAW, pge::semiOpaque(pge::colors::WHITE));

  auto login = std::make_unique<DecalScreenRenderer>(LOGIN_TEXTURE_FILE_PATH);
  login->loadResources(screenDims.x, screenDims.y, engine.getTextureHandler());
  m_renderers[Screen::LOGIN] = std::move(login);

  auto loading = std::make_unique<DecalScreenRenderer>(LOADING_TEXTURE_FILE_PATH);
  loading->loadResources(screenDims.x, screenDims.y, engine.getTextureHandler());
  m_renderers[Screen::LOADING] = std::move(loading);

  m_networkClient->start(m_serverPort);
}

void GameApp::cleanResources()
{
  m_networkClient->stop();

  m_renderers.clear();

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

void GameApp::drawUi(const pge::RenderState & /*state*/) {}

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

} // namespace bsgalone::client
