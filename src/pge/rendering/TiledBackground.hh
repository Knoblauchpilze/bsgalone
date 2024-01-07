
#pragma once

#include "RenderState.hh"
#include "SpriteRenderer.hh"
#include "TexturePack.hh"
#include "olcPixelGameEngine.h"
#include <memory>
#include <optional>
#include <string>

namespace pge {

class TiledBackground
{
  public:
  TiledBackground(const olc::vi2d &offset,
                  const int pixelSize,
                  const float slowdownRatio,
                  sprites::TexturePack &texturesLoader);

  void render(SpriteRenderer &spriteHandler, const RenderState &state);

  private:
  olc::vi2d m_offset{};
  static constexpr auto DEFAULT_BACKGROUND_SIZE_IN_PIXELS = 100;
  int m_pixelSize{DEFAULT_BACKGROUND_SIZE_IN_PIXELS};
  static constexpr auto DEFAULT_SLOWDOWN_FACTOR = 100.0f;
  float m_slowdownRatio{DEFAULT_SLOWDOWN_FACTOR};

  sprites::PackId m_bgTexturePackId{};

  std::optional<Vec2f> m_savedCenter{};
  std::optional<Vec2f> m_savedTileDimension{};
  olc::vf2d m_accumulatedTranslation{};

  void loadDecal(sprites::TexturePack &texturesLoader);
  void updateBackgroundOffset(const CoordinateFrame &cf);
  void renderBackgroundTile(const olc::vi2d &pixelPosition,
                            SpriteRenderer &spriteHandler,
                            const CoordinateFrame &cf);
};

using TiledBackgroundPtr = std::unique_ptr<TiledBackground>;

} // namespace pge
