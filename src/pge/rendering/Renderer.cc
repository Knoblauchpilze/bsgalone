
#include "Renderer.hh"
#include "VectorConversion.hh"
#include "olcEngine.hh"

namespace pge {

Renderer::Renderer(olc::PixelGameEngine *const renderer)
  : m_renderer(renderer)
{
  if (nullptr == m_renderer)
  {
    throw std::invalid_argument("Expected non null instance of PixelGameEngine");
  }

  m_packs = std::make_unique<sprites::TexturePack>(m_renderer);
}

auto Renderer::getRenderer() const -> olc::PixelGameEngine *
{
  return m_renderer;
}

auto Renderer::getTextureHandler() noexcept -> sprites::TexturePack &
{
  return *m_packs;
}

void Renderer::drawSprite(const SpriteDesc &t, const CoordinateFrame &cf)
{
  const auto p = cf.tilesToPixels(t.x, t.y);
  m_packs->draw(t.sprite, p, t.radius * cf.tileSize());
}

void Renderer::drawWarpedSprite(const SpriteDesc &t, const CoordinateFrame &cf)
{
  const auto p0 = cf.tilesToPixels(t.x, t.y + t.radius);
  const auto p1 = cf.tilesToPixels(t.x, t.y);
  const auto p2 = cf.tilesToPixels(t.x + t.radius, t.y);
  const auto p3 = cf.tilesToPixels(t.x + t.radius, t.y + t.radius);

  const auto p = std::array<Vec2f, 4>{p0, p1, p2, p3};
  m_packs->draw(t.sprite, p);
}

void Renderer::drawRotatedSprite(const SpriteDesc &t, const CoordinateFrame &cf)
{
  const auto p = cf.tilesToPixels(t.x, t.y);
  m_packs->draw(t.sprite, p, t.radius * cf.tileSize(), t.rotation);
}

void Renderer::drawRect(const SpriteDesc &t, const CoordinateFrame &cf)
{
  auto p = cf.tilesToPixels(t.x, t.y);
  // The FillRect draws in screen space below the input position. We want it
  // the other way around.
  p.y -= cf.tileSize().y;
  m_renderer->FillRectDecal(toVf2d(p), t.radius * toVf2d(cf.tileSize()), t.sprite.tint);
}

void Renderer::drawWarpedRect(const SpriteDesc &t, const CoordinateFrame &cf)
{
  const auto p0 = toVf2d(cf.tilesToPixels(t.x, t.y + t.radius));
  const auto p1 = toVf2d(cf.tilesToPixels(t.x, t.y));
  const auto p2 = toVf2d(cf.tilesToPixels(t.x + t.radius, t.y));
  const auto p3 = toVf2d(cf.tilesToPixels(t.x + t.radius, t.y + t.radius));

  // See: FillRectDecal(const olc::vf2d& pos, const olc::vf2d& size, const olc::Pixel col)
  // in the pixel game engine file.
  const auto p = std::array<olc::vf2d, 4>{p0, p1, p2, p3};
  std::array<olc::vf2d, 4> uvs;
  uvs.fill({0, 0});

  std::array<olc::Pixel, 4> colors;
  colors.fill(t.sprite.tint);

  m_renderer->DrawExplicitDecal(nullptr, p.data(), uvs.data(), colors.data());
}

} // namespace pge
