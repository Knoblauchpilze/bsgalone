
#pragma once

#include "Asteroid.hh"
#include "IGame.hh"
#include "IRenderer.hh"

namespace bsgalone::client {

class AsteroidRenderer : public IRenderer
{
  public:
  AsteroidRenderer(IGameShPtr game);
  ~AsteroidRenderer() override = default;

  void loadResources(const pge::Vec2i &dimensions,
                     pge::sprites::ITexturePack &texturesLoader) override;
  void render(pge::Renderer &engine,
              const pge::RenderState &state,
              const pge::RenderingPass pass) const override;

  private:
  IGameShPtr m_game{};

  pge::sprites::PackId m_asteroidTexturesPackId{};

  void loadAsteroidResources(pge::sprites::ITexturePack &texturesLoader);

  void renderAsteroid(const core::Asteroid &asteroid,
                      pge::Renderer &engine,
                      const pge::RenderState &state) const;
};

} // namespace bsgalone::client
