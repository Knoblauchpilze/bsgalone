
#include "App.hh"

namespace bsgalone::client {

App::App(const pge::AppDesc &desc)
  : PGEApp(desc)
{}

bool App::onFrame(const float /*elapsedSeconds*/)
{
  const auto maybeHandler = m_uiHandlers.find(m_screen);
  if (maybeHandler != m_uiHandlers.end())
  {
    maybeHandler->second->updateUi();
  }

  return m_screen == Screen::EXIT;
}

void App::onInputs(const pge::controls::State &controls, pge::CoordinateFrame &frame)
{
  const auto maybeHandler = m_inputHandlers.find(m_screen);
  if (maybeHandler == m_inputHandlers.end())
  {
    return;
  }

  maybeHandler->second->processUserInput(controls, frame);
}

void App::loadResources(const pge::Vec2i & /*screenDims*/, pge::Renderer & /*engine*/)
{
  setLayerTint(Layer::DRAW, semiOpaque(pge::colors::WHITE));
}

void App::cleanResources()
{
  m_inputHandlers.clear();
  m_uiHandlers.clear();
  m_renderers.clear();
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
  const auto maybeRenderer = m_renderers.find(m_screen);
  if (maybeRenderer == m_renderers.end())
  {
    return;
  }

  maybeRenderer->second->render(state.renderer, state, pge::RenderingPass::UI);
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

} // namespace bsgalone::client
