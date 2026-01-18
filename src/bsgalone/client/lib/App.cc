
#include "App.hh"

namespace bsgalone::client {

App::App(const pge::AppDesc &desc, const int serverPort)
  : PGEApp(desc)
  , m_serverPort(serverPort)
{}

App::App(const pge::AppDesc &desc,
         const int serverPort,
         const std::optional<std::string> &userName,
         const std::optional<std::string> &password)
  : PGEApp(desc)
  , m_serverPort(serverPort)
  , m_userName(userName)
  , m_password(password)
{}

bool App::onFrame(const float /*elapsedSeconds*/)
{
  return false;
}

void App::onInputs(const pge::controls::State & /*controls*/, pge::CoordinateFrame & /*frame*/) {}

void App::loadResources(const pge::Vec2i & /*screenDims*/, pge::Renderer & /*engine*/)
{
  setLayerTint(Layer::DRAW, semiOpaque(pge::colors::WHITE));
}

void App::cleanResources() {}

void App::drawDecal(const pge::RenderState & /*state*/) {}

void App::draw(const pge::RenderState & /*state*/) {}

void App::drawUi(const pge::RenderState & /*state*/) {}

void App::drawDebug(const pge::RenderState & /*state*/, const pge::Vec2f & /*mouseScreenPos*/) {}

} // namespace bsgalone::client
