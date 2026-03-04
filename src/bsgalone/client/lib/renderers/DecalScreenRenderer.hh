
#pragma once

#include "DecalResource.hh"
#include "IRenderer.hh"

namespace bsgalone::client {

class DecalScreenRenderer : public IRenderer
{
  public:
  DecalScreenRenderer(const std::string &decalFileName);
  ~DecalScreenRenderer() override = default;

  void loadResources(const int width,
                     const int height,
                     pge::sprites::TexturePack &texturesLoader) override;
  void render(pge::Renderer &engine,
              const pge::RenderState &state,
              const pge::RenderingPass pass) const override;

  private:
  std::string m_decalFileName{};
  pge::DecalResourcePtr m_bgDecal{};
  pge::Vec2f m_scale{1.0f, 1.0f};
};

} // namespace bsgalone::client
