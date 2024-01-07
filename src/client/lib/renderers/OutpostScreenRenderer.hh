
#pragma once

#include "DecalResource.hh"
#include "IRenderer.hh"

namespace pge {

class OutpostScreenRenderer : public IRenderer
{
  public:
  OutpostScreenRenderer();
  ~OutpostScreenRenderer() override = default;

  void loadResources(int width, int height, sprites::TexturePack &texturesLoader) override;
  void render(Renderer &engine, const RenderState &state, const RenderingPass pass) const override;

  private:
  DecalResourcePtr m_outpostDecal{};
  olc::vf2d m_scale{1.0f, 1.0f};
};

} // namespace pge
