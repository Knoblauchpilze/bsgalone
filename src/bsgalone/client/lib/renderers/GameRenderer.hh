
#pragma once

#include "DecalResource.hh"
#include "IRenderer.hh"
#include "TiledBackground.hh"

namespace bsgalone::client {

class GameRenderer : public IRenderer
{
  public:
  GameRenderer()           = default;
  ~GameRenderer() override = default;

  void loadResources(const pge::Vec2i &dimensions,
                     pge::sprites::ITexturePack &texturesLoader) override;
  void render(pge::Renderer &engine,
              const pge::RenderState &state,
              const pge::RenderingPass pass) const override;

  private:
  pge::TiledBackgroundPtr m_systemBackground{};
};

} // namespace bsgalone::client
