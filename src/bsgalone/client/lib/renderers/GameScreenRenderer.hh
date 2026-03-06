
#pragma once

#include "AbstractRenderer.hh"
#include "Entity.hh"
#include "TiledBackground.hh"
#include "Views.hh"

namespace bsgalone::client {

class GameScreenRenderer : public AbstractRenderer
{
  public:
  GameScreenRenderer(const Views &views);
  ~GameScreenRenderer() override = default;

  void loadResources(int width, int height, pge::sprites::TexturePack &texturesLoader) override;
  void render(pge::Renderer &engine,
              const pge::RenderState &state,
              const pge::RenderingPass pass) const override;

  private:
  ShipViewShPtr m_shipView;
  SystemViewShPtr m_systemView;
  ResourceViewShPtr m_resourceView;

  pge::TiledBackgroundPtr m_systemBackground{};

  pge::sprites::PackId m_asteroidTexturesPackId{};
  pge::sprites::PackId m_class1TexturesPackId{};
  pge::sprites::PackId m_class3TexturesPackId{};
  pge::sprites::PackId m_outpostTexturesPackId{};
  pge::sprites::PackId m_bulletTexturesPackId{};

  void renderDecal(pge::Renderer &engine, const pge::RenderState &state) const;
  void renderDebug(pge::Renderer &engine, const pge::RenderState &state) const;

  void renderAsteroid(const core::Entity &asteroid,
                      pge::Renderer &engine,
                      const pge::RenderState &state) const;
  void renderOutpost(const core::Entity &outpost,
                     pge::Renderer &engine,
                     const pge::RenderState &state) const;
  void renderBullet(const core::Entity &bullet,
                    pge::Renderer &engine,
                    const pge::RenderState &state) const;
  void renderShip(const core::Entity &ship,
                  pge::Renderer &engine,
                  const pge::RenderState &state) const;
  void renderShipDebug(const core::Entity &ship,
                       pge::Renderer &engine,
                       const pge::RenderState &state) const;
};

} // namespace bsgalone::client
