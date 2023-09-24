
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
                       const olc::vf2d &scale) const
{
  // Check whether the pack is valid.
  if (s.pack >= static_cast<int>(m_packs.size()))
  {
    log("Invalid pack " + std::to_string(s.pack) + " to draw sprite", utils::Level::Error);
    return;
  }

  const Pack &tp = m_packs[s.pack];

  olc::vi2d sCoords = tp.spriteCoords(s.sprite, s.id);
  pge->DrawPartialDecal(p, tp.res, sCoords, tp.sSize, scale, s.tint);
}

void TexturePack::draw(olc::PixelGameEngine *pge,
                       const Sprite &s,
                       const std::array<olc::vf2d, 4> &p) const
{
  // Check whether the pack is valid.
  if (s.pack >= static_cast<int>(m_packs.size()))
  {
    log("Invalid pack " + std::to_string(s.pack) + " to draw sprite", utils::Level::Error);
    return;
  }

  const Pack &tp = m_packs[s.pack];

  olc::vi2d sCoords = tp.spriteCoords(s.sprite, s.id);
  pge->DrawPartialWarpedDecal(tp.res, p, sCoords, tp.sSize, s.tint);
}

} // namespace pge::sprites
