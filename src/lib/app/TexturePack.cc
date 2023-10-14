
#include "TexturePack.hh"

namespace pge::sprites {

inline auto TexturePack::Pack::spriteCoords(const olc::vi2d &coord, int id) const -> olc::vi2d
{
  const auto lID = coord.y * layout.x + coord.x + id;
  // Go back to 2D coordinates using the layout on the linearized ID and the
  // size of the sprite to obtain a pixels position.
  return olc::vi2d((lID % layout.x) * sSize.x, (lID / layout.x) * sSize.y);
}

TexturePack::TexturePack()
  : utils::CoreObject("pack")
  , m_packs()
{
  setService("textures");
}

auto TexturePack::loadDecal(const std::string &fileName) -> DecalResourcePtr
{
  auto sprite = new olc::Sprite(fileName);
  if (nullptr == sprite)
  {
    error("Failed to create decal from \"" + fileName + "\"", "Failed to create sprite");
  }

  const auto decal = new olc::Decal(sprite);
  return std::make_unique<DecalResource>(decal);
}

auto TexturePack::registerPack(const PackDesc &pack) -> PackId
{
  Pack p;
  p.sSize  = pack.sSize;
  p.layout = pack.layout;

  p.decal = loadDecal(pack.file);

  const auto id = static_cast<PackId>(m_packs.size());
  m_packs.push_back(std::move(p));

  return id;
}

void TexturePack::draw(olc::PixelGameEngine *pge,
                       const sprites::Sprite &s,
                       const olc::vf2d &p,
                       const olc::vf2d &size) const
{
  const auto &tp        = tryGetPackOrThrow(s.pack);
  const auto sCoords    = tp.spriteCoords(s.sprite, s.id);
  const olc::vf2d scale = size / tp.sSize;
  pge->DrawPartialDecal(p, tp.decal->get(), sCoords, tp.sSize, scale, s.tint);
}

void TexturePack::draw(olc::PixelGameEngine *pge,
                       const Sprite &s,
                       const std::array<olc::vf2d, 4> &p) const
{
  const auto &tp     = tryGetPackOrThrow(s.pack);
  const auto sCoords = tp.spriteCoords(s.sprite, s.id);
  pge->DrawPartialWarpedDecal(tp.decal->get(), p, sCoords, tp.sSize, s.tint);
}

void TexturePack::draw(olc::PixelGameEngine *pge,
                       const Sprite &s,
                       const olc::vf2d &p,
                       const olc::vf2d &size,
                       const float angle) const
{
  const auto &tp        = tryGetPackOrThrow(s.pack);
  const auto sCoords    = tp.spriteCoords(s.sprite, s.id);
  const auto sCenter    = sCoords + tp.sSize / 2;
  const olc::vf2d scale = size / tp.sSize;
  // The minus comes from the interpretation of the PixelGameEngine to
  // make it consistent to provide a direct rotation. See this page in
  // French for more details: https://fr.wikipedia.org/wiki/Sens_de_rotation
  const auto olcAngle = angle;
  pge->DrawPartialRotatedDecal(p,
                               tp.decal->get(),
                               olcAngle,
                               sCenter,
                               sCoords,
                               tp.sSize,
                               scale,
                               s.tint);
}

auto TexturePack::tryGetPackOrThrow(const int packId) const -> const Pack &
{
  if (packId >= static_cast<int>(m_packs.size()))
  {
    error("Can't draw sprite", "Unknown pack id" + std::to_string(packId));
  }

  return m_packs[packId];
}

} // namespace pge::sprites
