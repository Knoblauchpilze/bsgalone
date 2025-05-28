
#pragma once

#include "Color.hh"
#include "CoreObject.hh"
#include "DecalResource.hh"
#include "Vector2d.hh"
#include <memory>

namespace olc {
class PixelGameEngine;
}

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

class TexturePack : public core::CoreObject
{
  public:
  TexturePack(olc::PixelGameEngine *const engine);
  ~TexturePack() = default;

  /// @brief - Attempt to load the sprite in the specified file name into a decal.
  /// The memory management is the responsibility of the caller.
  /// @param fileName - the name of the sprite to load.
  /// @return - the decal built from the input file.
  auto loadDecal(const std::string &fileName) -> DecalResourcePtr;

  /// @brief - Performs the registration of the input pack and return the
  /// corresponding pack identifier so that the caller can refer to this pack
  /// afterwards.
  /// @param pack - the pack to load.
  /// @return - an identifier allowing to reference this pack for later use.
  auto registerPack(const PackDesc &pack) -> PackId;

  /// @brief - Draw the sprite at the specified position and size.
  /// @param s - the sprite to draw.
  /// @param p - the position where the sprite will be drawn.
  /// @param size - defines the desired size of the sprite in pixels.
  void draw(const Sprite &s, const Vec2f &p, const Vec2f &size) const;

  /// @brief - Draw the sprite at the specified position and size.
  /// @param pge - the engine to use to perform the rendering.
  /// @param s - the sprite to draw.
  /// @param p - the position of the corners for the sprite.
  void draw(const Sprite &s, const std::array<Vec2f, 4> &p) const;

  /// @brief - Draw the sprite at the specified position and size.
  /// @param pge - the engine to use to perform the rendering.
  /// @param s - the sprite to draw.
  /// @param p - the position where the sprite will be drawn.
  /// @param size - defines the desired size of the sprite in pixels.
  /// @param angle - the angle with which the sprite is rotated.
  void draw(const Sprite &s, const Vec2f &p, const Vec2f &size, const float angle) const;

  private:
  /// @brief Internal representation of a loaded texture pack.
  struct Pack
  {
    // Defines the size in pixels of an individual sprite in the pack.
    Vec2i sSize;

    // The layout of the sprites in the pack. This defines how many
    // sprites are available in the pack.
    Vec2i layout;

    // The raw data pointing to the sprites.
    DecalResourcePtr decal;

    /// @brief - Used to convert from sprite coordinates to the corresponding pixels
    /// coordinates. This method should mostly be used to locate a sprite in a resource
    /// pack. The internal layout allows to know in advance how sprites are laid out in
    /// the pack and thus find the correct location based on the coordinates of the
    /// sprite and its identifier.
    /// In order to find the correct sprite, both some coordinates and a variation id
    /// should be set to fix a single element in the sprites.
    /// @param pack - the texture pack to which the coordinates correspond to.
    /// @param coord - the coordinates of the sprite to convert to pixels in the resource
    /// pack.
    /// @param id - the index of the variation of the sprite to use: default is `0`.
    /// @return - a vector representing the pixels coordinates for the input sprite coords.
    auto spriteCoords(const Vec2i &coord, const int id = 0) const -> Vec2i;
  };

  auto tryGetPackOrThrow(const int packId) const -> const Pack &;

  private:
  /// @brief - The list of packs registered so far for this object. Note that the
  /// identifier of each pack corresponds to the position of the pack in this vector.
  std::vector<Pack> m_packs{};

  olc::PixelGameEngine *const m_engine{};
};

using TexturePackPtr = std::unique_ptr<TexturePack>;

} // namespace pge::sprites
