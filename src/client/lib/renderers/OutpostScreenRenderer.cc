
#include "OutpostScreenRenderer.hh"

namespace pge {

constexpr auto OUTPOST_TEXTURE_FILE_PATH = "data/assets/outpost_bg.png";

OutpostScreenRenderer::OutpostScreenRenderer()
  : IRenderer("outpost")
{}

void OutpostScreenRenderer::loadResources(int width,
                                          int height,
                                          sprites::TexturePack &texturesLoader)
{
  m_outpostDecal = texturesLoader.loadDecal(OUTPOST_TEXTURE_FILE_PATH);
  m_scale.x      = 1.0f * width / m_outpostDecal->w();
  m_scale.y      = 1.0f * height / m_outpostDecal->h();
}

void OutpostScreenRenderer::render(SpriteRenderer &engine,
                                   const RenderState & /*state*/,
                                   const RenderingPass pass) const
{
  if (RenderingPass::DECAL != pass)
  {
    return;
  }

  const auto pge = engine.getRenderer();
  pge->DrawDecal(olc::vf2d{}, m_outpostDecal->get(), m_scale);
}

} // namespace pge
