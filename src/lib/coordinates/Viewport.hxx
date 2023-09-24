
#pragma once

#include "Viewport.hh"

namespace pge {

inline Viewport::Vectorf Viewport::relativeCoords(const Vectorf &pos) const noexcept
{
  return relativeCoords(pos.x, pos.y);
}

inline Viewport::Vectorf Viewport::absoluteCoords(const Vectorf &pos) const noexcept
{
  return absoluteCoords(pos.x, pos.y);
}

inline void Viewport::scale(const float factor) noexcept
{
  scale(factor, factor);
}

inline bool Viewport::visible(const float x, const float y, const float radius) const noexcept
{
  return visible(x, y, radius, radius);
}

inline bool Viewport::visible(const Vectorf &p, const Vectorf &sz) const noexcept
{
  return visible(p.x, p.y, sz.x, sz.y);
}

} // namespace pge
