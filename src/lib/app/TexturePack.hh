
#pragma once

#include "olcEngine.hh"
#include <core_utils/CoreObject.hh>
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
  olc::vi2d sSize;

  // Defines the dimensions of the pack in sprites. This helps to
  // load the relevant content and only consider portions of the
  // file if needed.
  olc::vi2d layout;
};

/// @brief - An identifier for a texture pack.
using PackId = int;

/// @brief - Convenience structure regrouping needed props to draw a sprite.
struct Sprite
{
  // The `pack` defines the identifier of the pack from
  // which the sprite should be picked.
  PackId pack;

  // The `sprite` defines an identifier for the sprite. The
  // position of the sprite in the resource pack will be
  // computed from this identifier.
  olc::vi2d sprite;

  // The `id` allows to select a variant for the sprite. By
  // default this value is `0` meaning the principal display
  // for the sprite.
  int id{0};

  // The `tint` defines a color to apply to tint the sprite
  // as a whole. Can also be used to provide some alpha.
  olc::Pixel tint{olc::WHITE};
};

class TexturePack : public utils::CoreObject
{
  public:
  /// @brief - Generate a new texture pack with no resources registered yet.
  TexturePack();

  /// @brief - Detroys the texture pack and release the sprites and resources
  /// attached to it.
  ~TexturePack();

  /// @brief - Performs the registration of the input pack and return the
  /// corresponding pack identifier so that the caller can refer to this pack
  /// afterwards.
  /// @param pack - the pack to load.
  /// @return - an identifier allowing to reference this pack for later use.
  PackId registerPack(const PackDesc &pack);

  /// @brief - Used to perform the drawing of the sprite as defined by the input
  /// argument using the engine. The sprite will be associated internally with
  /// the corresponding visual.
  /// @param pge - the engine to use to perform the rendering.
  /// @param s - the sprite to draw.
  /// @param p - the position where the sprite will be drawn.
  /// @param scale - defines a scaling factor to apply to the sprite.
  void draw(olc::PixelGameEngine *pge,
            const Sprite &s,
            const olc::vf2d &p,
            const olc::vf2d &scale = olc::vf2d(1.0f, 1.0f)) const;

  /// @brief - Used to perform the drawing of the sprite as defined by the input
  /// argument using the engine. The sprite will be associated internally with
  /// the corresponding visual and warped to match the list of positions for the
  /// corners.
  /// @param pge - the engine to use to perform the rendering.
  /// @param s - the sprite to draw.
  /// @param p - the position of the corners for the sprite.
  void draw(olc::PixelGameEngine *pge, const Sprite &s, const std::array<olc::vf2d, 4> &p) const;

  private:
  /// @brief Internal representation of a loaded texture pack.
  struct Pack
  {
    // Defines the size in pixels of an individual sprite in the pack.
    olc::vi2d sSize;

    // The layout of the sprites in the pack. This defines how many
    // sprites are available in the pack.
    olc::vi2d layout;

    // The raw data pointing to the sprites.
    olc::Decal *res;

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
    olc::vi2d spriteCoords(const olc::vi2d &coord, int id = 0) const;
  };

  private:
  /// @brief - The list of packs registered so far for this object. Note that the
  /// identifier of each pack corresponds to the position of the pack in this vector.
  std::vector<Pack> m_packs;
};

using TexturePackPtr = std::unique_ptr<TexturePack>;
} // namespace pge::sprites
