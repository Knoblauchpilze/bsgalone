
#include "TexturePack.hh"
#include "ColorConversion.hh"
#include "VectorConversion.hh"

namespace pge::sprites {

inline auto TexturePack::Pack::spriteCoords(const Vec2i &coord, int id) const -> Vec2i
{
  const auto lID = coord.y * layout.x + coord.x + id;
  // Go back to 2D coordinates using the layout on the linearized ID and the
  // size of the sprite to obtain a pixels position.
  return Vec2i{(lID % layout.x) * sSize.x, (lID / layout.x) * sSize.y};
}

TexturePack::TexturePack(olc::PixelGameEngine *const engine)
  : utils::CoreObject("pack")
  , m_packs()
  , m_engine(engine)
{
  setService("textures");

  if (nullptr == m_engine)
  {
    throw std::invalid_argument("Expected non null instance of PixelGameEngine");
  }
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

void TexturePack::draw(const sprites::Sprite &s, const Vec2f &p, const Vec2f &size) const
{
  const auto &tp     = tryGetPackOrThrow(s.pack);
  const auto sCoords = tp.spriteCoords(s.sprite, s.id);

  const olc::vf2d scale{size.x / tp.sSize.x, size.y / tp.sSize.y};
  m_engine->DrawPartialDecal(toVf2d(p),
                             tp.decal->get(),
                             toVi2d(sCoords),
                             toVi2d(tp.sSize),
                             scale,
                             colors::toOlcPixel(s.tint));
}

void TexturePack::draw(const Sprite &s, const std::array<Vec2f, 4> &p) const
{
  const auto &tp     = tryGetPackOrThrow(s.pack);
  const auto sCoords = tp.spriteCoords(s.sprite, s.id);

  const std::array<olc::vf2d, 4> olcArray{toVf2d(p[0]), toVf2d(p[1]), toVf2d(p[2]), toVf2d(p[3])};
  m_engine->DrawPartialWarpedDecal(tp.decal->get(),
                                   olcArray,
                                   toVi2d(sCoords),
                                   toVi2d(tp.sSize),
                                   colors::toOlcPixel(s.tint));
}

void TexturePack::draw(const Sprite &s, const Vec2f &p, const Vec2f &size, const float angle) const
{
  const auto &tp     = tryGetPackOrThrow(s.pack);
  const auto sCoords = tp.spriteCoords(s.sprite, s.id);
  const auto sCenter = sCoords + tp.sSize / 2;

  const olc::vf2d scale{size.x / tp.sSize.x, size.y / tp.sSize.y};
  m_engine->DrawPartialRotatedDecal(toVf2d(p),
                                    tp.decal->get(),
                                    angle,
                                    toVi2d(sCenter),
                                    toVi2d(sCoords),
                                    toVi2d(tp.sSize),
                                    scale,
                                    colors::toOlcPixel(s.tint));
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
