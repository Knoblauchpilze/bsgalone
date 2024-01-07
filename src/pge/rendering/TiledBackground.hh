
#pragma once

#include "RenderState.hh"
#include "SpriteRenderer.hh"
#include "TexturePack.hh"
#include "Vector2d.hh"
#include <memory>
#include <optional>
#include <string>

namespace pge {

class TiledBackground
{
  public:
  TiledBackground(const Vec2i &offset,
                  const int pixelSize,
                  const float slowdownRatio,
                  sprites::TexturePack &texturesLoader);

  void render(SpriteRenderer &spriteHandler, const RenderState &state);

  private:
  Vec2i m_offset{};
  static constexpr auto DEFAULT_BACKGROUND_SIZE_IN_PIXELS = 100;
  int m_pixelSize{DEFAULT_BACKGROUND_SIZE_IN_PIXELS};
  static constexpr auto DEFAULT_SLOWDOWN_FACTOR = 100.0f;
  float m_slowdownRatio{DEFAULT_SLOWDOWN_FACTOR};

  sprites::PackId m_bgTexturePackId{};

  std::optional<Vec2f> m_savedCenter{};
  std::optional<Vec2f> m_savedTileDimension{};
  Vec2f m_accumulatedTranslation{};

  void loadDecal(sprites::TexturePack &texturesLoader);
  void updateBackgroundOffset(const CoordinateFrame &cf);
  void renderBackgroundTile(const Vec2i &pixelPosition,
                            SpriteRenderer &spriteHandler,
                            const CoordinateFrame &cf);
};

using TiledBackgroundPtr = std::unique_ptr<TiledBackground>;

} // namespace pge
