
#include "GameRenderer.hh"

namespace pge {

GameRenderer::GameRenderer(const bsgo::GameViewShPtr gameView, int width, int height)
  : m_gameView(gameView)
{
  if (nullptr == m_gameView)
  {
    throw std::invalid_argument("Expected non null game view");
  }
  create(width, height);
}

void GameRenderer::render(SpriteRenderer & /*engine*/,
                          const RenderState & /*state*/,
                          const RenderingPass /*pass*/) const
{}

auto GameRenderer::processUserInput(const controls::State & /*c*/,
                                    std::vector<ActionShPtr> & /*actions*/) -> menu::InputHandle
{
  return {};
}

void GameRenderer::create(int /*width*/, int /*height*/) {}

} // namespace pge
