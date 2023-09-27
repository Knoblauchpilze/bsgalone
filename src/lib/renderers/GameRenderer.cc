
#include "GameRenderer.hh"
#include "ScreenCommon.hh"

namespace pge {

GameRenderer::GameRenderer(const bsgo::Views &views, int width, int height)
{
  create(views, width, height);
}

void GameRenderer::render(SpriteRenderer &engine,
                          const RenderState &state,
                          const RenderingPass pass) const
{
  switch (pass)
  {
    case RenderingPass::UI:
      m_uiRenderer->render(engine, state, pass);
      break;
    default:
      break;
  }
}

auto GameRenderer::processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
  -> menu::InputHandle
{
  return m_uiRenderer->processUserInput(c, actions);
}

void GameRenderer::updateUi()
{
  m_uiRenderer->updateUi();
}

void GameRenderer::create(const bsgo::Views &views, int width, int height)
{
  m_uiRenderer = std::make_unique<GameUiRenderer>(views, width, height);
}

} // namespace pge
