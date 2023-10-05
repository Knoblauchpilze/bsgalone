
#include "TexturePack.hh"

namespace pge::sprites {

inline olc::vi2d TexturePack::Pack::spriteCoords(const olc::vi2d &coord, int id) const
{
  int lID = coord.y * layout.x + coord.x + id;

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

TexturePack::~TexturePack()
{
  for (unsigned id = 0; id < m_packs.size(); ++id)
  {
    if (m_packs[id].res != nullptr)
    {
      delete m_packs[id].res;
    }
  }

  m_packs.clear();
}

PackId TexturePack::registerPack(const PackDesc &pack)
{
  // Load the file as a sprite and then convert it
  // to a faster `Decal` resource.
  olc::Sprite *spr = new olc::Sprite(pack.file);
  if (spr == nullptr)
  {
    error("Failed to load texture pack \"" + pack.file + "\"", "Loading returned null");
  }

  // Build the internal structure, register it and
  // return the corresponding identifier.
  Pack p;
  p.sSize  = pack.sSize;
  p.layout = pack.layout;

  p.res = new olc::Decal(spr);

  auto id = static_cast<PackId>(m_packs.size());
  m_packs.push_back(p);

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
  pge->DrawPartialDecal(p, tp.res, sCoords, tp.sSize, scale, s.tint);
}

void TexturePack::draw(olc::PixelGameEngine *pge,
                       const Sprite &s,
                       const std::array<olc::vf2d, 4> &p) const
{
  const auto &tp     = tryGetPackOrThrow(s.pack);
  const auto sCoords = tp.spriteCoords(s.sprite, s.id);
  pge->DrawPartialWarpedDecal(tp.res, p, sCoords, tp.sSize, s.tint);
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
  pge->DrawPartialRotatedDecal(p, tp.res, olcAngle, sCenter, sCoords, tp.sSize, scale, s.tint);
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
