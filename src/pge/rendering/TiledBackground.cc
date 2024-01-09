
#include "TiledBackground.hh"

namespace pge {

TiledBackground::TiledBackground(const Vec2i &offset,
                                 const int pixelSize,
                                 const float slowdownRatio,
                                 sprites::TexturePack &texturesLoader)
  : m_offset(offset)
  , m_pixelSize(pixelSize)
  , m_slowdownRatio(slowdownRatio)
{
  loadDecal(texturesLoader);
}

void TiledBackground::render(Renderer &engine, const RenderState &state)
{
  const auto pixelsViewport = state.frame.pixelsViewport();

  Vec2i bgTileCount;
  bgTileCount.x = static_cast<int>(std::ceil(pixelsViewport.dims().x / m_pixelSize));
  bgTileCount.y = static_cast<int>(std::ceil(pixelsViewport.dims().x / m_pixelSize));

  updateBackgroundOffset(state.frame);

  for (auto y = -1; y <= bgTileCount.y; ++y)
  {
    for (auto x = -1; x <= bgTileCount.x; ++x)
    {
      Vec2i pixelPos = m_offset;
      pixelPos.x += (x * m_pixelSize);
      pixelPos.y += (y * m_pixelSize);

      renderBackgroundTile(pixelPos, engine, state.frame);
    }
  }
}

void TiledBackground::loadDecal(sprites::TexturePack &texturesLoader)
{
  constexpr auto BACKGROUND_TEXTURE_PACK_FILE_PATH = "data/assets/system_bg.png";
  constexpr auto BACKGROUND_TILE_WIDTH_PIXELS      = 768;
  constexpr auto BACKGROUND_TILE_HEIGHT_PIXELS     = 768;
  auto pack = sprites::PackDesc{.file = BACKGROUND_TEXTURE_PACK_FILE_PATH,
                                .sSize{BACKGROUND_TILE_WIDTH_PIXELS, BACKGROUND_TILE_HEIGHT_PIXELS},
                                .layout{1, 1}};

  m_bgTexturePackId = texturesLoader.registerPack(pack);
}

void TiledBackground::updateBackgroundOffset(const CoordinateFrame &frame)
{
  const auto newTileSize = frame.tileSize();
  if (!m_savedCenter || !m_savedTileDimension || newTileSize.x != m_savedTileDimension->x
      || newTileSize.y != m_savedTileDimension->y)
  {
    m_savedCenter            = frame.tilesViewport().center();
    m_savedTileDimension     = newTileSize;
    m_accumulatedTranslation = {};
    m_offset                 = m_offset;
    return;
  }

  const auto newCenter = frame.tilesViewport().center();
  m_accumulatedTranslation.x += ((m_savedCenter->x - newCenter.x) / m_slowdownRatio);
  m_accumulatedTranslation.y += ((newCenter.y - m_savedCenter->y) / m_slowdownRatio);

  auto delta = static_cast<int>(std::floor(m_accumulatedTranslation.x));
  m_offset.x += delta;
  m_accumulatedTranslation.x -= delta;

  delta = static_cast<int>(std::floor(m_accumulatedTranslation.y));
  m_offset.y += delta;
  m_accumulatedTranslation.y -= delta;

  m_savedCenter = newCenter;

  m_offset.x = m_offset.x % m_pixelSize;
  m_offset.y = m_offset.y % m_pixelSize;
}

void TiledBackground::renderBackgroundTile(const Vec2i &pixelPosition,
                                           Renderer &engine,
                                           const CoordinateFrame &frame)
{
  const auto tilePosition = frame.pixelsToTiles(pixelPosition.x, pixelPosition.y);

  SpriteDesc t;
  t.x = tilePosition.x;
  t.y = tilePosition.y;

  t.radius = m_pixelSize / frame.tileSize().x;

  t.sprite.pack = m_bgTexturePackId;

  engine.drawWarpedSprite(t, frame);
}

} // namespace pge
