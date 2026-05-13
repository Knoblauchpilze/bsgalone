
#include "GameRenderer.hh"

namespace bsgalone::client {

void GameRenderer::loadResources(const pge::Vec2i & /*dimensions*/,
                                 pge::sprites::ITexturePack &texturesLoader)
{
  const pge::Vec2i offset{10, 10};
  constexpr auto BACKGROUND_TILE_SIZE_IN_PIXELS = 768;
  constexpr auto BACKGROUND_SLOWDOWN_RATIO      = 2.0f;
  m_systemBackground                            = std::make_unique<pge::TiledBackground>(offset,
                                                              BACKGROUND_TILE_SIZE_IN_PIXELS,
                                                              BACKGROUND_SLOWDOWN_RATIO,
                                                              texturesLoader);
}

void GameRenderer::render(pge::Renderer &engine,
                          const pge::RenderState &state,
                          const pge::RenderingPass pass) const
{
  if (pge::RenderingPass::DECAL == pass)
  {
    m_systemBackground->render(engine, state);
  }
}

} // namespace bsgalone::client
