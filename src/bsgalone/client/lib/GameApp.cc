
#include "GameApp.hh"
#include "DecalScreenRenderer.hh"

namespace bsgalone::client {

GameApp::GameApp(const pge::AppDesc &desc, const int serverPort)
  : pge::PGEApp(desc)
  , m_serverPort(serverPort)
{}

bool GameApp::onFrame(const float /*elapsedSeconds*/)
{
  return screen == Screen::EXIT;
}

void GameApp::onInputs(const pge::controls::State & /*controls*/, pge::CoordinateFrame & /*frame*/)
{}

void GameApp::loadResources(const pge::Vec2i &screenDims, pge::Renderer &engine)
{
  setLayerTint(Layer::DRAW, pge::semiOpaque(pge::colors::WHITE));

  constexpr auto LOGIN_BACKGROUND_PATH = "assets/login_bg.png";
  auto login = std::make_unique<DecalScreenRenderer>(LOGIN_BACKGROUND_PATH);
  login->loadResources(screenDims.x, screenDims.y, engine.getTextureHandler());
  m_renderers[Screen::LOGIN] = std::move(login);

  constexpr auto LOADING_BACKGROUND_PATH = "assets/loading_screen.png";
  auto loading = std::make_unique<DecalScreenRenderer>(LOADING_BACKGROUND_PATH);
  loading->loadResources(screenDims.x, screenDims.y, engine.getTextureHandler());
  m_renderers[Screen::LOADING] = std::move(loading);
}

void GameApp::cleanResources()
{
  m_renderers.clear();
}

void GameApp::drawDecal(const pge::RenderState &state)
{
  const auto it = m_renderers.find(screen);
  if (it != m_renderers.end())
  {
    it->second->render(state.renderer, state, pge::RenderingPass::DECAL);
  }
}

void GameApp::draw(const pge::RenderState & /*state*/) {}

void GameApp::drawUi(const pge::RenderState & /*state*/) {}

void GameApp::drawDebug(const pge::RenderState &state, const pge::Vec2f & /*mouseScreenPos*/)
{
  const auto it = m_renderers.find(screen);
  if (it != m_renderers.end())
  {
    it->second->render(state.renderer, state, pge::RenderingPass::DEBUG);
  }
}

} // namespace bsgalone::client
