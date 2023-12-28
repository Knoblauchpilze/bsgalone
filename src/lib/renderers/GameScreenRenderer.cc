
#include "GameScreenRenderer.hh"
#include "EntitiesUtils.hh"
#include "GameColorUtils.hh"
#include "ScreenCommon.hh"
#include "StringUtils.hh"

namespace pge {

GameScreenRenderer::GameScreenRenderer(const bsgo::Views &views)
  : IRenderer("game")
  , m_shipView(views.shipView)
  , m_systemView(views.systemView)
  , m_resourceView(views.resourceView)
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
  if (nullptr == m_systemView)
  {
    throw std::invalid_argument("Expected non null system view");
  }
  if (nullptr == m_resourceView)
  {
    throw std::invalid_argument("Expected non null resource view");
  }
}

void GameScreenRenderer::loadResources(int /*width*/,
                                       int /*height*/,
                                       sprites::TexturePack &texturesLoader)
{
  constexpr auto ASTEROID_TEXTURE_PACK_FILE_PATH = "data/assets/asteroid.png";
  constexpr auto ASTEROID_TILE_WIDTH_PIXELS      = 450;
  constexpr auto ASTEROID_TILE_HEIGHT_PIXELS     = 404;
  auto pack = sprites::PackDesc{.file = ASTEROID_TEXTURE_PACK_FILE_PATH,
                                .sSize{ASTEROID_TILE_WIDTH_PIXELS, ASTEROID_TILE_HEIGHT_PIXELS},
                                .layout{1, 1}};

  m_asteroidTexturesPackId = texturesLoader.registerPack(pack);

  constexpr auto CLASS_1_TEXTURE_PACK_FILE_PATH = "data/assets/class_1.png";
  constexpr auto CLASS_1_TILE_WIDTH_PIXELS      = 168;
  constexpr auto CLASS_1_TILE_HEIGHT_PIXELS     = 188;
  pack = sprites::PackDesc{.file = CLASS_1_TEXTURE_PACK_FILE_PATH,
                           .sSize{CLASS_1_TILE_WIDTH_PIXELS, CLASS_1_TILE_HEIGHT_PIXELS},
                           .layout{1, 1}};

  m_class1TexturesPackId = texturesLoader.registerPack(pack);

  constexpr auto CLASS_3_TEXTURE_PACK_FILE_PATH = "data/assets/class_3.png";
  constexpr auto CLASS_3_TILE_WIDTH_PIXELS      = 388;
  constexpr auto CLASS_3_TILE_HEIGHT_PIXELS     = 504;
  pack = sprites::PackDesc{.file = CLASS_3_TEXTURE_PACK_FILE_PATH,
                           .sSize{CLASS_3_TILE_WIDTH_PIXELS, CLASS_3_TILE_HEIGHT_PIXELS},
                           .layout{1, 1}};

  m_class3TexturesPackId = texturesLoader.registerPack(pack);

  constexpr auto OUTPOST_TEXTURE_PACK_FILE_PATH = "data/assets/outpost.png";
  constexpr auto OUTPOST_TILE_WIDTH_PIXELS      = 535;
  constexpr auto OUTPOST_TILE_HEIGHT_PIXELS     = 575;
  pack = sprites::PackDesc{.file = OUTPOST_TEXTURE_PACK_FILE_PATH,
                           .sSize{OUTPOST_TILE_WIDTH_PIXELS, OUTPOST_TILE_HEIGHT_PIXELS},
                           .layout{2, 1}};

  m_outpostTexturesPackId = texturesLoader.registerPack(pack);

  constexpr auto BULLET_TEXTURE_PACK_FILE_PATH = "data/assets/bullet.png";
  constexpr auto BULLET_TILE_WIDTH_PIXELS      = 485;
  constexpr auto BULLET_TILE_HEIGHT_PIXELS     = 492;
  pack = sprites::PackDesc{.file = BULLET_TEXTURE_PACK_FILE_PATH,
                           .sSize{BULLET_TILE_WIDTH_PIXELS, BULLET_TILE_HEIGHT_PIXELS},
                           .layout{1, 1}};

  m_bulletTexturesPackId = texturesLoader.registerPack(pack);

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
  const auto outposts  = m_systemView->getOutpostsWithin(bbox);
  const auto bullets   = m_systemView->getBulletsWithin(bbox);
  const auto ships     = m_shipView->getShipsWithin(bbox);

  for (const auto &asteroid : asteroids)
  {
    renderAsteroid(asteroid, engine, state);
  }

  for (const auto &outpost : outposts)
  {
    renderOutpost(outpost, engine, state);
  }

  for (const auto &bullet : bullets)
  {
    renderBullet(bullet, engine, state);
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

void GameScreenRenderer::renderAsteroid(const bsgo::Entity &asteroid,
                                        SpriteRenderer &engine,
                                        const RenderState &state) const
{
  const auto &transform   = asteroid.transformComp();
  const auto pos          = transform.position();
  const auto scanned      = asteroid.scannedComp().scanned();
  const auto containsLoot = asteroid.exists<bsgo::LootComponent>();

  SpriteDesc t;
  t.x = pos(0) - transform.size();
  t.y = pos(1) - transform.size();

  t.radius = 2.0f * transform.size();

  t.sprite.pack   = m_asteroidTexturesPackId;
  t.sprite.sprite = {0, 0};

  auto tint = olc::WHITE;
  if (scanned && !containsLoot)
  {
    tint = olc::RED;
  }
  else if (scanned && containsLoot)
  {
    if (1u != asteroid.resources.size())
    {
      error("Failed to render asteroid " + bsgo::str(asteroid.uuid),
            "Asteroid contains " + std::to_string(asteroid.resources.size()) + " resource(s)");
    }

    const auto res = m_resourceView->getResourceName(asteroid.resources[0]->resource());
    tint           = colorFromResourceName(res);
  }
  t.sprite.tint = tint;

  engine.drawWarpedSprite(t, state.cf);
}

namespace {
auto tintFromFaction(const bsgo::Faction &faction) -> olc::Pixel
{
  switch (faction)
  {
    case bsgo::Faction::COLONIAL:
      return olc::Pixel{153, 193, 241};
    case bsgo::Faction::CYLON:
      return olc::Pixel{191, 64, 64};
    default:
      return olc::RED;
  }
}
} // namespace

void GameScreenRenderer::renderOutpost(const bsgo::Entity &outpost,
                                       SpriteRenderer &engine,
                                       const RenderState &state) const
{
  const auto &transform = outpost.transformComp();
  const auto pos        = transform.position();

  SpriteDesc t;
  t.x = pos(0) - transform.size();
  t.y = pos(1) - transform.size();

  t.radius = 2.0f * transform.size();

  t.sprite.pack   = m_outpostTexturesPackId;
  const auto id   = outpost.factionComp().faction() == bsgo::Faction::COLONIAL ? 1 : 0;
  t.sprite.sprite = {id, 0};
  t.sprite.tint   = tintFromFaction(outpost.factionComp().faction());

  engine.drawWarpedSprite(t, state.cf);
}

void GameScreenRenderer::renderBullet(const bsgo::Entity &bullet,
                                      SpriteRenderer &engine,
                                      const RenderState &state) const
{
  const auto &transform = bullet.transformComp();
  const auto pos        = transform.position();

  SpriteDesc t;
  t.x = pos(0) - transform.size();
  t.y = pos(1) - transform.size();

  t.radius = 2.0f * transform.size();

  t.sprite.pack   = m_bulletTexturesPackId;
  t.sprite.sprite = {0, 0};
  t.sprite.tint   = olc::WHITE;

  engine.drawWarpedSprite(t, state.cf);
}

void GameScreenRenderer::renderShip(const bsgo::Entity &ship,
                                    SpriteRenderer &engine,
                                    const RenderState &state) const
{
  const auto &transform = ship.transformComp();
  const auto pos        = transform.position();

  SpriteDesc t;
  t.x = pos(0);
  t.y = pos(1);

  t.radius   = 2.0f * transform.size();
  t.rotation = transform.heading();

  const auto shipClass = ship.shipClassComp().shipClass();
  t.sprite.pack        = bsgo::ShipClass::STRIKE == shipClass ? m_class1TexturesPackId
                                                              : m_class3TexturesPackId;
  t.sprite.sprite      = {0, 0};

  auto tint = tintFromFaction(ship.factionComp().faction());

  const auto status = ship.statusComp().status();
  switch (status)
  {
    case bsgo::Status::VISIBLE:
    case bsgo::Status::THREAT:
    case bsgo::Status::JUMP:
    case bsgo::Status::JUMP_THREAT:
      t.sprite.tint = tint;
      break;
    case bsgo::Status::DOCKED:
    case bsgo::Status::DEAD:
      t.sprite.tint = olc::BLANK;
      break;
    case bsgo::Status::APPEARING:
    case bsgo::Status::JUMP_APPEARING:
      t.sprite.tint = transparent(tint, alpha::SemiOpaque);
      break;
    default:
      error("Failed to render ship", "Unknown status " + str(status));
      break;
  }

  engine.drawRotatedSprite(t, state.cf);
}

void GameScreenRenderer::renderShipDebug(const bsgo::Entity &ship,
                                         SpriteRenderer &engine,
                                         const RenderState &state) const
{
  const auto tilePos = ship.transformComp().position();
  olc::vi2d pixelPos = state.cf.tilesToPixels(tilePos(0), tilePos(1));

  olc::vi2d pos    = pixelPos;
  std::string text = "accel: ";
  const auto accel = ship.velocityComp().acceleration();
  text += floatToStr(accel(0));
  text += "x";
  text += floatToStr(accel(1));
  text += "x";
  text += floatToStr(accel(2));
  engine.getRenderer()->DrawString(pos, text, olc::DARK_YELLOW);

  constexpr auto REASONABLE_GAP = 20;
  pos.y += REASONABLE_GAP;
  text             = "speed: ";
  const auto speed = ship.velocityComp().speed();
  text += floatToStr(speed(0));
  text += "x";
  text += floatToStr(speed(1));
  text += "x";
  text += floatToStr(speed(2));
  engine.getRenderer()->DrawString(pos, text, olc::DARK_GREEN);
}

} // namespace pge
