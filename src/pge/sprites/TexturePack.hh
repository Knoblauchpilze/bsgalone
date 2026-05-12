
#pragma once

#include "CoreObject.hh"
#include "ITexturePack.hh"
#include <memory>

namespace olc {
class PixelGameEngine;
}

namespace pge::sprites {

class TexturePack : public ITexturePack, public core::CoreObject
{
  public:
  TexturePack(olc::PixelGameEngine *const engine);
  ~TexturePack() override = default;

  auto loadDecal(const std::string &fileName) -> DecalResourcePtr override;

  auto registerPack(const PackDesc &pack) -> PackId override;

  void draw(const Sprite &s, const Vec2f &p, const Vec2f &size) const override;
  void draw(const Sprite &s, const std::array<Vec2f, 4> &p) const override;
  void draw(const Sprite &s, const Vec2f &p, const Vec2f &size, const float angle) const override;

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

} // namespace pge::sprites
