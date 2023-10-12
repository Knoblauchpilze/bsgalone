
#include "GameScreenRenderer.hh"
#include "EntitiesUtils.hh"
#include "ScreenCommon.hh"
#include "StringUtils.hh"

namespace pge {

GameScreenRenderer::GameScreenRenderer(const bsgo::Views &views)
  : IRenderer("game")
  , m_shipView(views.shipView)
  , m_systemView(views.systemView)
  , m_targetView(views.targetView)
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
  if (nullptr == m_systemView)
  {
    throw std::invalid_argument("Expected non null system view");
  }
  if (nullptr == m_targetView)
  {
    throw std::invalid_argument("Expected non null target view");
  }
}

void GameScreenRenderer::loadResources(int /*width*/,
                                       int /*height*/,
                                       sprites::TexturePack &texturesLoader)
{
  constexpr auto CLASS_1_TEXTURE_PACK_FILE_PATH = "data/assets/class_1.png";
  constexpr auto CLASS_1_TILE_WIDTH_PIXELS      = 460;
  constexpr auto CLASS_1_TILE_HEIGHT_PIXELS     = 783;
  auto pack = sprites::PackDesc{.file = CLASS_1_TEXTURE_PACK_FILE_PATH,
                                .sSize{CLASS_1_TILE_WIDTH_PIXELS, CLASS_1_TILE_HEIGHT_PIXELS},
                                .layout{1, 1}};

  m_class1TexturesPackId = texturesLoader.registerPack(pack);

  constexpr auto ASTEROID_TEXTURE_PACK_FILE_PATH = "data/assets/asteroid.png";
  constexpr auto ASTEROID_TILE_WIDTH_PIXELS      = 450;
  constexpr auto ASTEROID_TILE_HEIGHT_PIXELS     = 404;
  pack = sprites::PackDesc{.file = ASTEROID_TEXTURE_PACK_FILE_PATH,
                           .sSize{ASTEROID_TILE_WIDTH_PIXELS, ASTEROID_TILE_HEIGHT_PIXELS},
                           .layout{1, 1}};

  m_asteroidTexturesPackId = texturesLoader.registerPack(pack);

  const olc::vi2d offset{10, 10};
  constexpr auto BACKGROUND_TILE_SIZE_IN_PIXELS = 768;
  constexpr auto BACKGROUND_SLOWDOWN_RATIO      = 2.0f;
  m_systemBackground                            = std::make_unique<TiledBackground>(offset,
                                                         BACKGROUND_TILE_SIZE_IN_PIXELS,
                                                         BACKGROUND_SLOWDOWN_RATIO,
                                                         texturesLoader);
}

void GameScreenRenderer::render(SpriteRenderer &engine,
                                const RenderState &state,
                                const RenderingPass pass) const
{
  switch (pass)
  {
    case RenderingPass::DECAL:
      renderDecal(engine, state);
      break;
    case RenderingPass::DEBUG:
      renderDebug(engine, state);
      break;
    default:
      break;
  }
}

void GameScreenRenderer::renderDecal(SpriteRenderer &engine, const RenderState &state) const
{
  m_systemBackground->render(engine, state);

  const auto bbox      = toIBoundingBox(state.cf);
  const auto asteroids = m_systemView->getAsteroidsWithin(bbox);
  const auto ships     = m_shipView->getShipsWithin(bbox);

  for (const auto &asteroid : asteroids)
  {
    renderAsteroid(asteroid.access<bsgo::Transform>().position(),
                   asteroid.access<bsgo::Transform>().size(),
                   engine,
                   state);
  }

  for (const auto &ship : ships)
  {
    renderShip(ship, engine, state);
  }
}

void GameScreenRenderer::renderDebug(SpriteRenderer &engine, const RenderState &state) const
{
  const auto bbox  = toIBoundingBox(state.cf);
  const auto ships = m_shipView->getShipsWithin(bbox);

  for (const auto &ship : ships)
  {
    renderShipDebug(ship, engine, state);
  }
}

void GameScreenRenderer::renderAsteroid(const Eigen::Vector3f &position,
                                        const float radius,
                                        SpriteRenderer &engine,
                                        const RenderState &state) const
{
  SpriteDesc t;
  t.x = position(0) - radius;
  t.y = position(1) - radius;

  t.radius = 2.0f * radius;

  t.sprite.pack   = m_asteroidTexturesPackId;
  t.sprite.sprite = {0, 0};
  t.sprite.tint   = olc::WHITE;

  engine.drawWarpedSprite(t, state.cf);
}

void GameScreenRenderer::renderShip(const bsgo::Entity &ship,
                                    SpriteRenderer &engine,
                                    const RenderState &state) const
{
  const auto &transform = ship.access<bsgo::Transform>();
  const auto pos        = transform.position();

  SpriteDesc t;
  t.x = pos(0);
  t.y = pos(1);

  t.radius         = 2.0f * transform.size();
  const auto speed = ship.access<bsgo::Velocity>().speed();
  t.rotation       = std::atan2(speed(0), speed(1));

  t.sprite.pack   = m_class1TexturesPackId;
  t.sprite.sprite = {0, 0};
  t.sprite.tint   = olc::WHITE;

  engine.drawRotatedSprite(t, state.cf);
}

void GameScreenRenderer::renderShipDebug(const bsgo::Entity &ship,
                                         SpriteRenderer &engine,
                                         const RenderState &state) const
{
  const auto tilePos = ship.access<bsgo::Transform>().position();
  olc::vi2d pixelPos = state.cf.tilesToPixels(tilePos(0), tilePos(1));

  olc::vi2d pos    = pixelPos;
  std::string text = "accel: ";
  const auto accel = ship.access<bsgo::Velocity>().acceleration();
  text += floatToStr(accel(0));
  text += "x";
  text += floatToStr(accel(1));
  text += "x";
  text += floatToStr(accel(2));
  engine.getRenderer()->DrawString(pos, text, olc::DARK_YELLOW);

  constexpr auto REASONABLE_GAP = 20;
  pos.y += REASONABLE_GAP;
  text             = "speed: ";
  const auto speed = ship.access<bsgo::Velocity>().speed();
  text += floatToStr(speed(0));
  text += "x";
  text += floatToStr(speed(1));
  text += "x";
  text += floatToStr(speed(2));
  engine.getRenderer()->DrawString(pos, text, olc::DARK_GREEN);
}

} // namespace pge
