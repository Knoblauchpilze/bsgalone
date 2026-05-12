
#pragma once

#include "Color.hh"
#include "DecalResource.hh"
#include "Vector2d.hh"
#include <memory>

namespace pge::sprites {

/// @brief - Describe a texture pack to be loaded.
struct PackDesc
{
  // Defines the path to the file containing the textures. It should
  // point to a valid location.
  std::string file;

  // Defines the size of a single texture inside the pack in pixels.
  // Should match the file layout otherwise the sprite indices will
  // not work correctly, resulting in truncated sprites.
  Vec2i sSize;

  // Defines the dimensions of the pack in sprites. This helps to
  // load the relevant content and only consider portions of the
  // file if needed.
  Vec2i layout;
};

/// @brief - An identifier for a texture pack.
using PackId = int;

/// @brief - Convenience structure regrouping needed props to draw a sprite.
struct Sprite
{
  // The `pack` defines the identifier of the pack from
  // which the sprite should be picked.
  PackId pack{0};

  // The `sprite` defines an identifier for the sprite. The
  // position of the sprite in the resource pack will be
  // computed from this identifier.
  Vec2i sprite{0, 0};

  // The `id` allows to select a variant for the sprite. By
  // default this value is `0` meaning the principal display
  // for the sprite.
  int id{0};

  // The `tint` defines a color to apply to tint the sprite
  // as a whole. Can also be used to provide some alpha.
  Color tint{colors::WHITE};
};

class ITexturePack
{
  public:
  virtual ~ITexturePack() = default;

  /// @brief - Attempt to load the sprite in the specified file name into a decal.
  /// The memory management is the responsibility of the caller.
  /// @param fileName - the name of the sprite to load.
  /// @return - the decal built from the input file.
  virtual auto loadDecal(const std::string &fileName) -> DecalResourcePtr = 0;

  /// @brief - Performs the registration of the input pack and return the
  /// corresponding pack identifier so that the caller can refer to this pack
  /// afterwards.
  /// @param pack - the pack to load.
  /// @return - an identifier allowing to reference this pack for later use.
  virtual auto registerPack(const PackDesc &pack) -> PackId = 0;

  /// @brief - Draw the sprite at the specified position and size.
  /// @param s - the sprite to draw.
  /// @param p - the position where the sprite will be drawn.
  /// @param size - defines the desired size of the sprite in pixels.
  virtual void draw(const Sprite &s, const Vec2f &p, const Vec2f &size) const = 0;

  /// @brief - Draw the sprite at the specified position and size.
  /// @param pge - the engine to use to perform the rendering.
  /// @param s - the sprite to draw.
  /// @param p - the position of the corners for the sprite.
  virtual void draw(const Sprite &s, const std::array<Vec2f, 4> &p) const = 0;

  /// @brief - Draw the sprite at the specified position and size.
  /// @param pge - the engine to use to perform the rendering.
  /// @param s - the sprite to draw.
  /// @param p - the position where the sprite will be drawn.
  /// @param size - defines the desired size of the sprite in pixels.
  /// @param angle - the angle with which the sprite is rotated.
  virtual void draw(const Sprite &s, const Vec2f &p, const Vec2f &size, const float angle) const = 0;
};

using ITexturePackPtr = std::unique_ptr<ITexturePack>;

} // namespace pge::sprites
