
#include "MapScreenRenderer.hh"

namespace pge {

constexpr auto MAP_TEXTURE_FILE_PATH = "data/assets/map_bg.png";

MapScreenRenderer::MapScreenRenderer()
  : IRenderer("map")
{}

void MapScreenRenderer::loadResources(int width, int height, sprites::TexturePack &texturesLoader)
{
  m_bgDecal = texturesLoader.loadDecal(MAP_TEXTURE_FILE_PATH);
  m_scale.x = 1.0f * width / m_bgDecal->w();
  m_scale.y = 1.0f * height / m_bgDecal->h();
}

void MapScreenRenderer::render(SpriteRenderer &engine,
                               const RenderState & /*state*/,
                               const RenderingPass pass) const
{
  if (RenderingPass::DECAL != pass)
  {
    return;
  }

  const auto pge = engine.getRenderer();
  pge->DrawDecal(olc::vf2d{}, m_bgDecal->get(), m_scale);
}

} // namespace pge
