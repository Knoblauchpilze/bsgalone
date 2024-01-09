
#include "RenderState.hh"

namespace pge {

bool RenderState::visible(const Vec2i &p, float r) const noexcept
{
  return frame.tilesViewport().visible(p.x, p.y, r, r);
}

bool RenderState::visible(const Vec2f &p, const Vec2f sz) const noexcept
{
  return frame.tilesViewport().visible(p.x, p.y, sz.x, sz.y);
}

} // namespace pge
