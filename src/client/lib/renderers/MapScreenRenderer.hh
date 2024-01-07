
#pragma once

#include "DecalResource.hh"
#include "IRenderer.hh"

namespace pge {

class MapScreenRenderer : public IRenderer
{
  public:
  MapScreenRenderer();
  ~MapScreenRenderer() override = default;

  void loadResources(int width, int height, sprites::TexturePack &texturesLoader) override;
  void render(Renderer &engine, const RenderState &state, const RenderingPass pass) const override;

  private:
  DecalResourcePtr m_bgDecal{};
  olc::vf2d m_scale{1.0f, 1.0f};
};

} // namespace pge
