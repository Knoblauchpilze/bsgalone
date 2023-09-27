
#pragma once

#include "CoordinateFrame.hh"
#include "TexturePack.hh"
#include "olcPixelGameEngine.h"
#include <memory>

namespace pge {

/// @brief - Convenience structure regrouping needed props to
/// draw a sprite.
struct SpriteDesc
{
  // The x coordinate of the sprite.
  float x;

  // The y coordinate of the sprite.
  float y;

  // The radius of the sprite: applied both along the x and y coordinates.
  float radius;

  // A description of the sprite.
  sprites::Sprite sprite;
};

class SpriteRenderer
{
  public:
  SpriteRenderer(olc::PixelGameEngine *renderer);

  auto getRenderer() const -> olc::PixelGameEngine *;
  auto getTextureHandler() noexcept -> sprites::TexturePack &;

  /// @brief - Used to draw the tile referenced by the input struct to the screen
  /// using the corresponding visual representation.
  /// @param t - the description of the tile to draw.
  /// @param cf - the coordinate frame to use to perform the conversion from tile
  /// position to pixels.
  void drawSprite(const SpriteDesc &t, const CoordinateFrame &cf);

  /// @brief - Render a warped sprite. The position of the sprite as defined in
  /// the input argument will be used to map the sprite to occupy exactly the part
  /// of the tile corresponding to the radius. For example if `t.x = 0, t.y = 0`,
  /// the sprite will be mapped to occupy the `(0, 0)` tile and extends up to the
  /// coordinate `(0 + radius, 0 + radius)`. It respects the current zoom and the
  /// the orientation of the tile given the coordinate frame.
  /// @param t - the description of the tile to draw.
  /// @param cf - the coordinate frame to use to perform the conversion from tile
  /// position to pixels.
  void drawWarpedSprite(const SpriteDesc &t, const CoordinateFrame &cf);

  /// @brief - Used to draw a simple rect at the specified location. Note that we
  /// reuse the sprite desc but don't actually use the sprite.
  /// @param t - the description of the tile to draw.
  /// @param cf - the coordinate frame to use to perform the conversion from tile
  /// position to pixels.
  void drawRect(const SpriteDesc &t, const CoordinateFrame &cf);

  /// @brief - Render a warped rect. Similar to what happens for `drawWarpedSprite`
  /// but with a rectangle instead of a sprite. Similarly we don't use the sprite
  /// defined in the `t` argument.
  /// @param t - the description of the rect to draw.
  /// @param cf - the coordinate frame to use to perform the conversion from tile
  /// position to pixels.
  void drawWarpedRect(const SpriteDesc &t, const CoordinateFrame &cf);

  private:
  olc::PixelGameEngine *m_renderer{nullptr};
  sprites::TexturePackPtr m_packs{std::make_unique<sprites::TexturePack>()};
};

using SpriteRendererPtr = std::unique_ptr<SpriteRenderer>;

} // namespace pge
