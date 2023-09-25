
#include "GameRenderer.hh"

namespace pge {

GameRenderer::GameRenderer(int width, int height)
{
  create(width, height);
}

void GameRenderer::render(SpriteRenderer & /*engine*/, const RenderState & /*state*/) const {}

auto GameRenderer::processUserInput(const controls::State & /*c*/,
                                    std::vector<ActionShPtr> & /*actions*/) -> menu::InputHandle
{
  return {};
}

void GameRenderer::create(int /*width*/, int /*height*/) {}

} // namespace pge
