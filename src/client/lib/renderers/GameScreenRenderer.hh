
#pragma once

#include "Entity.hh"
#include "IRenderer.hh"
#include "TiledBackground.hh"
#include "Views.hh"

namespace pge {

class GameScreenRenderer : public IRenderer
{
  public:
  GameScreenRenderer(const bsgo::Views &views);
  ~GameScreenRenderer() override = default;

  void loadResources(int width, int height, sprites::TexturePack &texturesLoader) override;
  void render(Renderer &engine, const RenderState &state, const RenderingPass pass) const override;

  private:
  bsgo::ShipViewShPtr m_shipView;
  bsgo::SystemViewShPtr m_systemView;
  bsgo::ResourceViewShPtr m_resourceView;

  TiledBackgroundPtr m_systemBackground{};

  sprites::PackId m_asteroidTexturesPackId{};
  sprites::PackId m_class1TexturesPackId{};
  sprites::PackId m_class3TexturesPackId{};
  sprites::PackId m_outpostTexturesPackId{};
  sprites::PackId m_bulletTexturesPackId{};

  void renderDecal(Renderer &engine, const RenderState &state) const;
  void renderDebug(Renderer &engine, const RenderState &state) const;

  void renderAsteroid(const bsgo::Entity &asteroid,
                      Renderer &engine,
                      const RenderState &state) const;
  void renderOutpost(const bsgo::Entity &outpost, Renderer &engine, const RenderState &state) const;
  void renderBullet(const bsgo::Entity &bullet, Renderer &engine, const RenderState &state) const;
  void renderShip(const bsgo::Entity &ship, Renderer &engine, const RenderState &state) const;
  void renderShipDebug(const bsgo::Entity &ship, Renderer &engine, const RenderState &state) const;
};

} // namespace pge
