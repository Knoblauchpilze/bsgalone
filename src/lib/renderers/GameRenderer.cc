
#include "GameRenderer.hh"
#include "ScreenCommon.hh"

namespace pge {

GameRenderer::GameRenderer(const bsgo::Views &views)
  : m_uiRenderer(std::make_unique<GameUiRenderer>(views))
  , m_systemRenderer(std::make_unique<GameSystemRenderer>(views))
{}

void GameRenderer::loadResources(int width, int height, sprites::TexturePack &texturesLoader)
{
  m_uiRenderer->loadResources(width, height, texturesLoader);
  m_systemRenderer->loadResources(width, height, texturesLoader);
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

} // namespace pge
