
#include "DecalScreenRenderer.hh"

namespace bsgalone::client {

DecalScreenRenderer::DecalScreenRenderer(const std::string &decalFileName)
  : IRenderer()
  , m_decalFileName(decalFileName)
{}

void DecalScreenRenderer::loadResources(const int width,
                                        const int height,
                                        pge::sprites::TexturePack &texturesLoader)
{
  m_bgDecal = texturesLoader.loadDecal(m_decalFileName);
  m_scale.x = 1.0f * width / m_bgDecal->w();
  m_scale.y = 1.0f * height / m_bgDecal->h();
}

void DecalScreenRenderer::render(pge::Renderer &engine,
                                 const pge::RenderState & /*state*/,
                                 const pge::RenderingPass pass) const
{
  if (pge::RenderingPass::DECAL != pass)
  {
    return;
  }

  engine.drawDecal({}, *m_bgDecal, m_scale);
}

} // namespace bsgalone::client
