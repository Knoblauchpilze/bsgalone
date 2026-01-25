
#include "GameApp.hh"

namespace bsgalone::client {

GameApp::GameApp(const pge::AppDesc &desc, const int serverPort)
  : pge::PGEApp(desc)
  , m_serverPort(serverPort)
{}

bool GameApp::onFrame(const float /*elapsedSeconds*/)
{
  return false;
}

void GameApp::onInputs(const pge::controls::State & /*controls*/, pge::CoordinateFrame & /*frame*/)
{}

void GameApp::loadResources(const pge::Vec2i & /*screenDims*/, pge::Renderer & /*engine*/)
{
  setLayerTint(Layer::DRAW, pge::semiOpaque(pge::colors::WHITE));
}

void GameApp::cleanResources() {}

void GameApp::drawDecal(const pge::RenderState & /*state*/) {}

void GameApp::draw(const pge::RenderState & /*state*/) {}

void GameApp::drawUi(const pge::RenderState & /*state*/) {}

void GameApp::drawDebug(const pge::RenderState & /*state*/, const pge::Vec2f & /*mouseScreenPos*/)
{}

} // namespace bsgalone::client
