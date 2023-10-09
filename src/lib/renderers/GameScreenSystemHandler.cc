
#include "GameScreenSystemHandler.hh"
#include "EntitiesUtils.hh"
#include "ScreenCommon.hh"

namespace pge {

GameScreenSystemHandler::GameScreenSystemHandler(const bsgo::Views &views)
  : m_systemView(views.systemView)
  , m_targetView(views.targetView)
{
  if (nullptr == m_systemView)
  {
    throw std::invalid_argument("Expected non null system view");
  }
  if (nullptr == m_targetView)
  {
    throw std::invalid_argument("Expected non null target view");
  }
}

void GameScreenSystemHandler::loadResources(int /*width*/,
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

  const olc::vi2d offset{10, 10};
  constexpr auto BACKGROUND_TILE_SIZE_IN_PIXELS = 768;
  constexpr auto BACKGROUND_SLOWDOWN_RATIO      = 2.0f;
  m_systemBackground                            = std::make_unique<TiledBackground>(offset,
                                                         BACKGROUND_TILE_SIZE_IN_PIXELS,
                                                         BACKGROUND_SLOWDOWN_RATIO,
                                                         texturesLoader);
}

void GameScreenSystemHandler::render(SpriteRenderer &engine,
                                     const RenderState &state,
                                     const RenderingPass pass) const
{
  if (pass != RenderingPass::DECAL)
  {
    return;
  }

  m_systemBackground->render(engine, state);

  const auto asteroids = getEntitiesWithinViewport(*m_systemView,
                                                   state.cf,
                                                   bsgo::EntityKind::ASTEROID);
  for (const auto &asteroid : asteroids)
  {
    renderAsteroid((*asteroid.transform)->position(), (*asteroid.transform)->size(), engine, state);
  }
}

auto GameScreenSystemHandler::processUserInput(const controls::State & /*c*/,
                                               std::vector<ActionShPtr> & /*actions*/,
                                               CoordinateFrame & /*frame*/) -> menu::InputHandle
{
  return {};
}

void GameScreenSystemHandler::updateUi() {}

void GameScreenSystemHandler::performAction(float x, float y, const controls::State & /*state*/)
{
  const Eigen::Vector3f pos(x, y, 0.0f);
  const auto ent = m_systemView->getEntityAt(pos);

  if (ent)
  {
    log("Found target " + ent->str());
    m_targetView->setTarget(ent->uuid);
  }
  else
  {
    m_targetView->clearTarget();
  }
}

void GameScreenSystemHandler::renderAsteroid(const Eigen::Vector3f &position,
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

} // namespace pge
