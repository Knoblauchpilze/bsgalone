
#include "DecalScreenRenderer.hh"

namespace pge {

DecalScreenRenderer::DecalScreenRenderer(const std::string &name, const std::string &decalFileName)
  : IRenderer(name)
  , m_decalFileName(decalFileName)
{}

void DecalScreenRenderer::loadResources(int width, int height, sprites::TexturePack &texturesLoader)
{
  m_bgDecal = texturesLoader.loadDecal(m_decalFileName);
  m_scale.x = 1.0f * width / m_bgDecal->w();
  m_scale.y = 1.0f * height / m_bgDecal->h();
}

void DecalScreenRenderer::render(Renderer &engine,
                                 const RenderState & /*state*/,
                                 const RenderingPass pass) const
{
  if (RenderingPass::DECAL != pass)
  {
    return;
  }

  engine.drawDecal({}, *m_bgDecal, m_scale);
}

} // namespace pge
