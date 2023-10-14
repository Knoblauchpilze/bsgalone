
#include "OutpostScreenRenderer.hh"

namespace pge {

constexpr auto OUTPOST_TEXTURE_PACK_FILE_PATH = "data/assets/outpost_bg.png";

OutpostScreenRenderer::OutpostScreenRenderer()
  : IRenderer("outpost")
{}

void OutpostScreenRenderer::loadResources(int width,
                                          int height,
                                          sprites::TexturePack &texturesLoader)
{
  m_outpostDecal = texturesLoader.loadDecal(OUTPOST_TEXTURE_PACK_FILE_PATH);
  m_scale.x      = 1.0f * width / m_outpostDecal->w();
  m_scale.y      = 1.0f * height / m_outpostDecal->h();
  log("dims: " + std::to_string(width) + "x" + std::to_string(height)
      + ", decal: " + std::to_string(m_outpostDecal->w()) + "x"
      + std::to_string(m_outpostDecal->h()) + ", scale: " + m_scale.str());
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
