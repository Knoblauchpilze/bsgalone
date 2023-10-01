
#include "GameScreenSystemHandler.hh"
#include "ScreenCommon.hh"

namespace pge {

GameScreenSystemHandler::GameScreenSystemHandler(const bsgo::Views &views)
  : m_systemView(views.systemView)
{
  if (nullptr == m_systemView)
  {
    throw std::invalid_argument("Expected non null system view");
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

  const auto asteroids = m_systemView->getAsteroidPositions();
  for (const auto &asteroidPos : asteroids)
  {
    renderAsteroid(asteroidPos, engine, state);
  }
}

auto GameScreenSystemHandler::processUserInput(const controls::State & /*c*/,
                                               std::vector<ActionShPtr> & /*actions*/)
  -> menu::InputHandle
{
  return {};
}

void GameScreenSystemHandler::updateUi() {}

void GameScreenSystemHandler::performAction(float /*x*/,
                                            float /*y*/,
                                            const controls::State & /*state*/)
{
  warn("should detect collision");
}

void GameScreenSystemHandler::renderAsteroid(const Eigen::Vector3f &position,
                                             SpriteRenderer &engine,
                                             const RenderState &state) const
{
  SpriteDesc t;
  t.x = position(0);
  t.y = position(1);

  t.radius = 1.0f;

  t.sprite.pack   = m_asteroidTexturesPackId;
  t.sprite.sprite = {0, 0};
  t.sprite.tint   = olc::WHITE;

  engine.drawWarpedSprite(t, state.cf);
}

} // namespace pge
