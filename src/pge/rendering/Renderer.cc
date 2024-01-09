
#include "Renderer.hh"
#include "ColorConversion.hh"
#include "VectorConversion.hh"

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

auto Renderer::getTextureHandler() noexcept -> sprites::TexturePack &
{
  return *m_packs;
}

void Renderer::drawSprite(const SpriteDesc &t, const CoordinateFrame &frame)
{
  const auto p = frame.tilesToPixels(t.x, t.y);
  m_packs->draw(t.sprite, p, t.radius * frame.tileSize());
}

void Renderer::drawWarpedSprite(const SpriteDesc &t, const CoordinateFrame &frame)
{
  const auto p0 = frame.tilesToPixels(t.x, t.y + t.radius);
  const auto p1 = frame.tilesToPixels(t.x, t.y);
  const auto p2 = frame.tilesToPixels(t.x + t.radius, t.y);
  const auto p3 = frame.tilesToPixels(t.x + t.radius, t.y + t.radius);

  const auto p = std::array<Vec2f, 4>{p0, p1, p2, p3};
  m_packs->draw(t.sprite, p);
}

void Renderer::drawRotatedSprite(const SpriteDesc &t, const CoordinateFrame &frame)
{
  const auto p = frame.tilesToPixels(t.x, t.y);
  m_packs->draw(t.sprite, p, t.radius * frame.tileSize(), t.rotation);
}

void Renderer::drawRect(const SpriteDesc &t, const CoordinateFrame &frame)
{
  auto p = frame.tilesToPixels(t.x, t.y);
  // The FillRect draws in screen space below the input position. We want it
  // the other way around.
  p.y -= frame.tileSize().y;
  m_renderer->FillRectDecal(toVf2d(p),
                            t.radius * toVf2d(frame.tileSize()),
                            colors::toOlcPixel(t.sprite.tint));
}

void Renderer::drawWarpedRect(const SpriteDesc &t, const CoordinateFrame &frame)
{
  const auto p0 = toVf2d(frame.tilesToPixels(t.x, t.y + t.radius));
  const auto p1 = toVf2d(frame.tilesToPixels(t.x, t.y));
  const auto p2 = toVf2d(frame.tilesToPixels(t.x + t.radius, t.y));
  const auto p3 = toVf2d(frame.tilesToPixels(t.x + t.radius, t.y + t.radius));

  // See: FillRectDecal(const olc::vf2d& pos, const olc::vf2d& size, const olc::Pixel col)
  // in the pixel game engine file.
  const auto p = std::array<olc::vf2d, 4>{p0, p1, p2, p3};
  std::array<olc::vf2d, 4> uvs;
  uvs.fill({0, 0});

  std::array<olc::Pixel, 4> colors;
  colors.fill(colors::toOlcPixel(t.sprite.tint));

  m_renderer->DrawExplicitDecal(nullptr, p.data(), uvs.data(), colors.data());
}

void Renderer::drawDecal(const Vec2f &pos,
                         const DecalResource &decal,
                         const Vec2f &scale,
                         const Color &tint)
{
  m_renderer->DrawDecal(toVf2d(pos), decal.get(), toVf2d(scale), colors::toOlcPixel(tint));
}

void Renderer::fillRect(const Vec2f &pos, const Vec2f &size, const Color tint)
{
  m_renderer->FillRectDecal(toVf2d(pos), toVf2d(size), colors::toOlcPixel(tint));
}

void Renderer::fillRect(const Vec2i &pos, const Vec2i &size, const Color tint)
{
  fillRect(toVec2f(pos), toVec2f(size), tint);
}

void Renderer::drawString(const Vec2f &pos,
                          const std::string &text,
                          const Color tint,
                          const Vec2f &scale)
{
  m_renderer->DrawStringDecal(toVf2d(pos), text, colors::toOlcPixel(tint), toVf2d(scale));
}

void Renderer::drawString(const Vec2i &pos,
                          const std::string &text,
                          const Color tint,
                          const Vec2f &scale)
{
  return drawString(toVec2f(pos), text, tint, scale);
}

void Renderer::drawDebugString(const Vec2i &pos, const std::string &text, const Color &tint)
{
  m_renderer->DrawString(toVi2d(pos), text, colors::toOlcPixel(tint));
}

auto Renderer::getTextSize(const std::string &text) const -> Vec2i
{
  return toVec2i(m_renderer->GetTextSize(text));
}

} // namespace pge
