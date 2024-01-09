
#pragma once

#include "CoordinateFrame.hh"
#include "TexturePack.hh"
#include <memory>

namespace olc {
class PixelGameEngine;
}

namespace pge {

/// @brief - Convenience structure regrouping needed props to
/// draw a sprite.
struct SpriteDesc
{
  float x{0.0f};
  float y{0.0f};
  float radius{1.0f};
  float rotation{0.0f};

  sprites::Sprite sprite;
};

class Renderer
{
  public:
  Renderer(olc::PixelGameEngine *const renderer);

  auto getTextureHandler() noexcept -> sprites::TexturePack &;

  /// @brief - Used to draw the tile referenced by the input struct to the screen
  /// using the corresponding visual representation.
  /// @param t - the description of the tile to draw.
  /// @param frame - the coordinate frame to use to perform the conversion from tile
  /// position to pixels.
  void drawSprite(const SpriteDesc &t, const CoordinateFrame &frame);

  /// @brief - Render a rotated sprite. The sprite will be rotated about its center.
  /// @param t - the description of the tile to draw.
  /// @param frame - the coordinate frame to use to perform the conversion from tile
  /// position to pixels.
  void drawRotatedSprite(const SpriteDesc &t, const CoordinateFrame &frame);

  /// @brief - Render a warped sprite. The position of the sprite as defined in
  /// the input argument will be used to map the sprite to occupy exactly the part
  /// of the tile corresponding to the radius. For example if `t.x = 0, t.y = 0`,
  /// the sprite will be mapped to occupy the `(0, 0)` tile and extends up to the
  /// coordinate `(0 + radius, 0 + radius)`. It respects the current zoom and the
  /// the orientation of the tile given the coordinate frame.
  /// @param t - the description of the tile to draw.
  /// @param frame - the coordinate frame to use to perform the conversion from tile
  /// position to pixels.
  void drawWarpedSprite(const SpriteDesc &t, const CoordinateFrame &frame);

  /// @brief - Used to draw a simple rect at the specified location. Note that we
  /// reuse the sprite desc but don't actually use the sprite.
  /// @param t - the description of the tile to draw.
  /// @param frame - the coordinate frame to use to perform the conversion from tile
  /// position to pixels.
  void drawRect(const SpriteDesc &t, const CoordinateFrame &frame);

  /// @brief - Render a warped rect. Similar to what happens for `drawWarpedSprite`
  /// but with a rectangle instead of a sprite. Similarly we don't use the sprite
  /// defined in the `t` argument.
  /// @param t - the description of the rect to draw.
  /// @param frame - the coordinate frame to use to perform the conversion from tile
  /// position to pixels.
  void drawWarpedRect(const SpriteDesc &t, const CoordinateFrame &frame);

  /// @brief - Render the decal with the specified position, scale and tint.
  /// @param pos - the position to render the decal at.
  /// @param decal - the resource to render.
  /// @param scale - additional scale to apply to the decal.
  /// @param tint - a tint to apply to the rendered object.
  void drawDecal(const Vec2f &pos,
                 const DecalResource &decal,
                 const Vec2f &scale,
                 const Color &tint = colors::WHITE);

  /// @brief - Render a simple rectangle with a flat color.
  /// @param pos - the position to render to (top left corner).
  /// @param size - the size of the patch of color.
  /// @param tint - the tint of the uniform patch of color.
  void fillRect(const Vec2f &pos, const Vec2f &size, const Color tint);

  void fillRect(const Vec2i &pos, const Vec2i &size, const Color tint);

  /// @brief - Render a string with the specified color.
  /// @param pos - the position to render the string at (top left corner).
  /// @param text - the text to render.
  /// @param tint - the color of the text.
  /// @param scale - additional scaling to apply to the text.
  void drawString(const Vec2f &pos,
                  const std::string &text,
                  const Color tint,
                  const Vec2f &scale = Vec2f(1.0f, 1.0f));

  void drawString(const Vec2i &pos,
                  const std::string &text,
                  const Color tint,
                  const Vec2f &scale = Vec2f(1.0f, 1.0f));

  /// @brief - Render a debug string in a slower way but which should be displayed
  /// on top of everything else.
  /// @param pos - the position to render the string at (top left corner).
  /// @param text - the text to render.
  /// @param tint - the color of the text.
  void drawDebugString(const Vec2i &pos, const std::string &text, const Color &tint);

  /// @brief - Return an estimation of the size the input text would occupy
  /// on screen if rendered.
  /// @param text - the text which size should be estimated.
  /// @return - an estimation of the size in pixels of the input text.
  auto getTextSize(const std::string &text) const -> Vec2i;

  private:
  olc::PixelGameEngine *m_renderer{nullptr};
  sprites::TexturePackPtr m_packs{};
};

using RendererPtr = std::unique_ptr<Renderer>;

} // namespace pge
