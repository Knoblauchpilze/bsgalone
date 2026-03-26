
#include "App.hh"

namespace bsgalone::client {

App::App(const pge::AppDesc &desc)
  : PGEApp(desc)
{}

bool App::onFrame(const float /*elapsedSeconds*/)
{
  return m_screen == Screen::EXIT;
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

void App::drawDebug(const pge::RenderState &state, const pge::Vec2f &mouseScreenPos)
{
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
