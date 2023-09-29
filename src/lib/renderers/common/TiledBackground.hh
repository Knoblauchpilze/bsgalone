
#pragma once

#include "RenderState.hh"
#include "SpriteRenderer.hh"
#include "TexturePack.hh"
#include "olcPixelGameEngine.h"
#include <memory>
#include <string>

namespace pge {

class TiledBackground
{
  public:
  TiledBackground(const int pixelSize, sprites::TexturePack &texturesLoader);

  void render(SpriteRenderer &spriteHandler, const RenderState &state);

  private:
  static constexpr auto DEFAULT_BACKGROUND_SIZE_IN_PIXELS = 100;
  int m_pixelSize{DEFAULT_BACKGROUND_SIZE_IN_PIXELS};
  sprites::PackId m_bgTexturePackId{};

  void loadDecal(sprites::TexturePack &texturesLoader);
  void renderTile(const olc::vf2d &position,
                  const float scale,
                  SpriteRenderer &spriteHandler,
                  const CoordinateFrame &cf);
};

using TiledBackgroundPtr = std::unique_ptr<TiledBackground>;

} // namespace pge
