
#include "GameShipRenderer.hh"
#include "ScreenCommon.hh"

namespace pge {

GameShipRenderer::GameShipRenderer(const bsgo::Views &views)
  : m_shipView(views.shipView)
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
}

void GameShipRenderer::loadResources(int /*width*/,
                                     int /*height*/,
                                     sprites::TexturePack & /*texturesLoader*/)
{}

void GameShipRenderer::render(SpriteRenderer & /*engine*/,
                              const RenderState & /*state*/,
                              const RenderingPass pass) const
{
  if (pass != RenderingPass::DECAL)
  {
    return;
  }
}

auto GameShipRenderer::processUserInput(const controls::State & /*c*/,
                                        std::vector<ActionShPtr> & /*actions*/) -> menu::InputHandle
{
  return {};
}

void GameShipRenderer::updateUi() {}

} // namespace pge
