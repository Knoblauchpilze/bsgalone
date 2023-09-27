
#include "GameSystemRenderer.hh"
#include "ScreenCommon.hh"

namespace pge {

GameSystemRenderer::GameSystemRenderer(const bsgo::Views &views)
  : m_systemView(views.systemView)
{
  if (nullptr == m_systemView)
  {
    throw std::invalid_argument("Expected non null system view");
  }
}

void GameSystemRenderer::loadResources(int /*width*/,
                                       int /*height*/,
                                       sprites::TexturePack & /*texturesLoader*/)
{}

void GameSystemRenderer::render(SpriteRenderer & /*engine*/,
                                const RenderState & /*state*/,
                                const RenderingPass pass) const
{
  if (pass != RenderingPass::DECAL)
  {
    return;
  }
}

auto GameSystemRenderer::processUserInput(const controls::State & /*c*/,
                                          std::vector<ActionShPtr> & /*actions*/)
  -> menu::InputHandle
{
  return {};
}

void GameSystemRenderer::updateUi() {}

} // namespace pge
