
#pragma once

#include "DecalResource.hh"
#include "IRenderer.hh"

namespace bsgalone::client {

class DecalRenderer : public IRenderer
{
  public:
  explicit DecalRenderer(const std::string &decalFileName);
  ~DecalRenderer() override = default;

  void loadResources(const pge::Vec2i &dimensions,
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
