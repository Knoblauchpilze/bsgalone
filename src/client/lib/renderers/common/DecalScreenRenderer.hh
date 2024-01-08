
#pragma once

#include "DecalResource.hh"
#include "IRenderer.hh"

namespace pge {

class DecalScreenRenderer : public IRenderer
{
  public:
  DecalScreenRenderer(const std::string &name, const std::string &decalFileName);
  ~DecalScreenRenderer() override = default;

  void loadResources(int width, int height, sprites::TexturePack &texturesLoader) override;
  void render(Renderer &engine, const RenderState &state, const RenderingPass pass) const override;

  private:
  std::string m_decalFileName{};
  DecalResourcePtr m_bgDecal{};
  Vec2f m_scale{1.0f, 1.0f};
};

} // namespace pge
