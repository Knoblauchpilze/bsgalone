
#include "AsteroidRenderer.hh"
#include "AxisAlignedBox.hh"

namespace bsgalone::client {

AsteroidRenderer::AsteroidRenderer(IGameShPtr game)
  : m_game(std::move(game))
{
  if (m_game == nullptr)
  {
    throw std::invalid_argument("Expected non null game");
  }
}

void AsteroidRenderer::loadResources(const pge::Vec2i & /*dimensions*/,
                                     pge::sprites::ITexturePack &texturesLoader)
{
  constexpr auto ASTEROID_TEXTURE_PACK_FILE_PATH = "assets/asteroid.png";
  constexpr auto ASTEROID_TILE_WIDTH_PIXELS      = 450;
  constexpr auto ASTEROID_TILE_HEIGHT_PIXELS     = 404;
  auto pack = pge::sprites::PackDesc{.file = ASTEROID_TEXTURE_PACK_FILE_PATH,
                                     .sSize{ASTEROID_TILE_WIDTH_PIXELS, ASTEROID_TILE_HEIGHT_PIXELS},
                                     .layout{1, 1}};

  m_asteroidTexturesPackId = texturesLoader.registerPack(pack);
}

namespace {
constexpr auto DEFAULT_Z = 0.0f;

auto toIBoundingBox(const pge::CoordinateFrame &frame) -> core::AxisAlignedBox
{
  const auto vpPos = frame.tilesViewport().center();
  const Eigen::Vector3f p(vpPos.x, vpPos.y, DEFAULT_Z);
  const auto vpDims = frame.tilesViewport().dims();
  const Eigen::Vector3f dims(vpDims.x, vpDims.y, DEFAULT_Z);
  return core::AxisAlignedBox(p, dims);
}
} // namespace

void AsteroidRenderer::render(pge::Renderer &engine,
                              const pge::RenderState &state,
                              const pge::RenderingPass pass) const
{
  if (pge::RenderingPass::DECAL != pass)
  {
    return;
  }

  const auto bbox = toIBoundingBox(state.frame);
  for (const auto &asteroid : m_game->getAsteroidsWithin(bbox))
  {
    renderAsteroid(asteroid, engine, state);
  }
}

void AsteroidRenderer::renderAsteroid(const core::Asteroid &asteroid,
                                      pge::Renderer &engine,
                                      const pge::RenderState &state) const
{
  pge::SpriteDesc t{
    .x      = asteroid.position(0) - asteroid.radius,
    .y      = asteroid.position(1) - asteroid.radius,
    .radius = 2.0f * asteroid.radius,
    .sprite = pge::sprites::Sprite{.pack   = m_asteroidTexturesPackId,
                                   .sprite = {0, 0},
                                   .tint  = pge::colors::WHITE,
                                },
  };

  engine.drawWarpedSprite(t, state.frame);
}

} // namespace bsgalone::client
