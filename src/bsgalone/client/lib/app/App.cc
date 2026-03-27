
#include "App.hh"
#include "AsyncUiCommandQueue.hh"
#include "LoginUiHandler.hh"
#include "SynchronizedUiCommandQueue.hh"

namespace bsgalone::client {

App::App(const pge::AppDesc &desc)
  : PGEApp(desc)
  , m_uiCommandQueue(createSynchronizedUiCommandQueue())
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

  generateUiHandlers(screenDims, engine.getTextureHandler());
}

void App::cleanResources()
{
  m_inputHandlers.clear();
  m_uiHandlers.clear();
  m_renderers.clear();

  m_uiCommandQueue.reset();
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

void App::generateUiHandlers(const pge::Vec2i &screenDims, pge::sprites::TexturePack &texturesLoader)
{
  auto login = std::make_unique<LoginUiHandler>(m_uiCommandQueue);
  login->initializeMenus(screenDims, texturesLoader);
  m_uiHandlers[Screen::LOGIN] = std::move(login);
}

} // namespace bsgalone::client
