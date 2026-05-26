
#include "GameRenderer.hh"

namespace bsgalone::client {

GameRenderer::GameRenderer(IGameShPtr game)
  : m_game(game)
  , m_asteroidRenderer(game)
{
  if (m_game == nullptr)
  {
    throw std::invalid_argument("Expected non null game");
  }
}

void GameRenderer::loadResources(const pge::Vec2i &dimensions,
                                 pge::sprites::ITexturePack &texturesLoader)
{
  const pge::Vec2i offset{10, 10};
  constexpr auto BACKGROUND_TILE_SIZE_IN_PIXELS = 768;
  constexpr auto BACKGROUND_SLOWDOWN_RATIO      = 2.0f;
  m_systemBackground                            = std::make_unique<pge::TiledBackground>(offset,
                                                              BACKGROUND_TILE_SIZE_IN_PIXELS,
                                                              BACKGROUND_SLOWDOWN_RATIO,
                                                              texturesLoader);

  m_asteroidRenderer.loadResources(dimensions, texturesLoader);
}

void GameRenderer::render(pge::Renderer &engine,
                          const pge::RenderState &state,
                          const pge::RenderingPass pass) const
{
  if (pge::RenderingPass::DECAL != pass)
  {
    return;
  }

  m_systemBackground->render(engine, state);
  m_asteroidRenderer.render(engine, state, pass);
}

} // namespace bsgalone::client
