
#include "App.hh"
#include "Screen.hh"

namespace pge {

App::App(const AppDesc &desc)
  : PGEApp(desc)
{}

bool App::onFrame(const float elapsedSeconds)
{
  // Handle case where no game is defined.
  if (m_game == nullptr)
  {
    return false;
  }

  if (!m_game->step(elapsedSeconds))
  {
    info("This is game over");
  }

  return m_game->terminated();
}

void App::onInputs(const controls::State &controls, CoordinateFrame &frame)
{
  // Handle case where no game is defined.
  if (m_game == nullptr)
  {
    return;
  }

  m_game->processUserInput(controls, frame);
}

void App::loadResources(const Vec2i &screenDims, Renderer &engine)
{
  setLayerTint(Layer::DRAW, semiOpaque(colors::WHITE));

  m_game = std::make_shared<Game>();

  m_game->generateRenderers(screenDims.x, screenDims.y, engine);
  m_game->generateInputHandlers();
  m_game->generateUiHandlers(screenDims.x, screenDims.y, engine);
}

void App::cleanResources()
{
  m_game.reset();
}

void App::drawDecal(const RenderState &state)
{
  m_game->render(state, RenderingPass::DECAL);
}

void App::draw(const RenderState &state)
{
  m_game->render(state, RenderingPass::SPRITES);
}

void App::drawUi(const RenderState &state)
{
  m_game->render(state, RenderingPass::UI);
}

void App::drawDebug(const RenderState &state, const Vec2f &mouseScreenPos)
{
  m_game->render(state, RenderingPass::DEBUG);

  Vec2f it;
  const auto mtp = state.frame.pixelsToTilesAndIntra(mouseScreenPos, &it);

  constexpr auto DEBUG_PIXELS_Y       = 400;
  constexpr auto REASONABLE_PIXEL_GAP = 15;
  Vec2i pos(0, DEBUG_PIXELS_Y);

  state.renderer.drawDebugString(pos, "Mouse coords      : " + mouseScreenPos.str(), colors::CYAN);
  pos.y += REASONABLE_PIXEL_GAP;
  state.renderer.drawDebugString(pos, "World cell coords : " + mtp.str(), colors::CYAN);
  pos.y += REASONABLE_PIXEL_GAP;
  state.renderer.drawDebugString(pos, "Intra cell        : " + it.str(), colors::CYAN);
  pos.y += REASONABLE_PIXEL_GAP;
  state.renderer.drawDebugString(pos,
                                 "Screen            : " + str(m_game->getScreen()),
                                 colors::DARK_GREEN);
}

} // namespace pge
