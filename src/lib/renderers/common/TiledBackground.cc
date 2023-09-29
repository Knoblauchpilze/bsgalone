
#include "TiledBackground.hh"

namespace pge {

TiledBackground::TiledBackground(const int pixelSize, sprites::TexturePack &texturesLoader)
  : m_pixelSize(pixelSize)
{
  loadDecal(texturesLoader);
}

void TiledBackground::render(SpriteRenderer &spriteHandler, const RenderState &state)
{
  const auto tilesViewport = state.cf.tilesViewport();
  const auto topLeft       = tilesViewport.topLeft();
  const auto dims          = tilesViewport.dims();

  const auto bgSizeInTile = m_pixelSize / state.cf.tileSize().x;

  olc::vi2d topLeftTile;
  topLeftTile.x = static_cast<int>(std::floor(topLeft.x / bgSizeInTile));
  topLeftTile.y = static_cast<int>(std::ceil(topLeft.y / bgSizeInTile));

  olc::vi2d bottomRightTile;
  bottomRightTile.x = static_cast<int>(std::ceil((topLeft.x + dims.x) / bgSizeInTile));
  bottomRightTile.y = static_cast<int>(std::floor((topLeft.y - dims.y) / bgSizeInTile));

  for (float y = bottomRightTile.y; y <= topLeftTile.y; ++y)
  {
    for (float x = topLeftTile.x; x <= bottomRightTile.x; ++x)
    {
      const olc::vf2d tilePos{x * bgSizeInTile, y * bgSizeInTile};
      renderTile(tilePos, bgSizeInTile, spriteHandler, state.cf);
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

void TiledBackground::renderTile(const olc::vf2d &position,
                                 const float scale,
                                 SpriteRenderer &spriteHandler,
                                 const CoordinateFrame &cf)
{
  SpriteDesc t;
  t.x = position.x;
  t.y = position.y;

  t.radius = scale;

  t.sprite.pack   = m_bgTexturePackId;
  t.sprite.sprite = {0, 0};
  t.sprite.tint   = olc::WHITE;

  spriteHandler.drawWarpedSprite(t, cf);
}

} // namespace pge
