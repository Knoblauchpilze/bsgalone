
#include "SpriteRenderer.hh"

namespace pge {

SpriteRenderer::SpriteRenderer(olc::PixelGameEngine *renderer)
  : m_renderer(renderer)
{
  if (nullptr == m_renderer)
  {
    throw std::invalid_argument("Expected non null instance of PixelGameEngine");
  }
}

auto SpriteRenderer::getRenderer() const -> olc::PixelGameEngine *
{
  return m_renderer;
}

auto SpriteRenderer::getTextureHandler() noexcept -> sprites::TexturePack &
{
  return *m_packs;
}

void SpriteRenderer::drawSprite(const SpriteDesc &t, const CoordinateFrame &cf)
{
  const auto p = cf.tilesToPixels(t.x, t.y);
  m_packs->draw(m_renderer, t.sprite, p, t.radius * cf.tileSize());
}

void SpriteRenderer::drawWarpedSprite(const SpriteDesc &t, const CoordinateFrame &cf)
{
  const auto p0 = cf.tilesToPixels(t.x, t.y + t.radius);
  const auto p1 = cf.tilesToPixels(t.x, t.y);
  const auto p2 = cf.tilesToPixels(t.x + t.radius, t.y);
  const auto p3 = cf.tilesToPixels(t.x + t.radius, t.y + t.radius);

  const auto p = std::array<olc::vf2d, 4>{p0, p1, p2, p3};
  m_packs->draw(m_renderer, t.sprite, p);
}

void SpriteRenderer::drawRotatedSprite(const SpriteDesc &t, const CoordinateFrame &cf)
{
  const auto p = cf.tilesToPixels(t.x, t.y);
  m_packs->draw(m_renderer, t.sprite, p, t.radius * cf.tileSize(), t.rotation);
}

void SpriteRenderer::drawRect(const SpriteDesc &t, const CoordinateFrame &cf)
{
  auto p = cf.tilesToPixels(t.x, t.y);
  // The FillRect draws in screen space below the input position. We want it
  // the other way around.
  p.y -= cf.tileSize().y;
  m_renderer->FillRectDecal(p, t.radius * cf.tileSize(), t.sprite.tint);
}

void SpriteRenderer::drawWarpedRect(const SpriteDesc &t, const CoordinateFrame &cf)
{
  const auto p0 = cf.tilesToPixels(t.x, t.y + t.radius);
  const auto p1 = cf.tilesToPixels(t.x, t.y);
  const auto p2 = cf.tilesToPixels(t.x + t.radius, t.y);
  const auto p3 = cf.tilesToPixels(t.x + t.radius, t.y + t.radius);

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