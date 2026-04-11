
#include "DecalRenderer.hh"

namespace bsgalone::client {

DecalRenderer::DecalRenderer(const std::string &decalFileName)
  : IRenderer()
  , m_decalFileName(decalFileName)
{}

void DecalRenderer::loadResources(const pge::Vec2i &dimensions,
                                  pge::sprites::TexturePack &texturesLoader)
{
  m_bgDecal = texturesLoader.loadDecal(m_decalFileName);
  m_scale.x = 1.0f * dimensions.x / m_bgDecal->w();
  m_scale.y = 1.0f * dimensions.y / m_bgDecal->h();
}

void DecalRenderer::render(pge::Renderer &engine,
                           const pge::RenderState & /*state*/,
                           const pge::RenderingPass pass) const
{
  if (pge::RenderingPass::DECAL == pass)
  {
    engine.drawDecal({}, *m_bgDecal, m_scale);
  }
}

} // namespace bsgalone::client
