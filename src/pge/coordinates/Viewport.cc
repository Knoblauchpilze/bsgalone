
#include "Viewport.hh"

namespace pge {

auto Viewport::relativeCoords(const Vec2f &pos) const noexcept -> Vec2f
{
  return relativeCoords(pos.x, pos.y);
}

auto Viewport::absoluteCoords(const Vec2f &pos) const noexcept -> Vec2f
{
  return absoluteCoords(pos.x, pos.y);
}

void Viewport::scale(const float factor) noexcept
{
  scale(factor, factor);
}

bool Viewport::visible(const float x, const float y, const float radius) const noexcept
{
  return visible(x, y, radius, radius);
}

bool Viewport::visible(const Vec2f &p, const Vec2f &sz) const noexcept
{
  return visible(p.x, p.y, sz.x, sz.y);
}

} // namespace pge
