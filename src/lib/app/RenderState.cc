
#include "RenderState.hh"

namespace pge {

bool RenderState::visible(const utils::Point2i &p, float r) const noexcept
{
  return cf.tilesViewport().visible(p.x(), p.y(), r, r);
}

bool RenderState::visible(const olc::vf2d &p, const olc::vf2d sz) const noexcept
{
  return cf.tilesViewport().visible(p.x, p.y, sz.x, sz.y);
}

} // namespace pge