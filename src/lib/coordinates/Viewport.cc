
#include "Viewport.hh"

namespace pge {

auto Viewport::relativeCoords(const Vectorf &pos) const noexcept -> Vectorf
{
  return relativeCoords(pos.x, pos.y);
}

auto Viewport::absoluteCoords(const Vectorf &pos) const noexcept -> Vectorf
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

bool Viewport::visible(const Vectorf &p, const Vectorf &sz) const noexcept
{
  return visible(p.x, p.y, sz.x, sz.y);
}

} // namespace pge
