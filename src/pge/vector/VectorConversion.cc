
#include "VectorConversion.hh"

namespace pge {

auto toVf2d(const Vec2f &v) -> olc::vf2d
{
  return olc::vf2d{v.x, v.y};
}

auto toVi2d(const Vec2i &v) -> olc::vi2d
{
  return olc::vi2d{v.x, v.y};
}

auto toVec2f(const olc::vf2d &v) -> Vec2f
{
  return Vec2f{v.x, v.y};
}

auto toVec2i(const olc::vi2d &v) -> Vec2i
{
  return Vec2i{v.x, v.y};
}

auto toVec2f(const Vec2i &v) -> Vec2f
{
  return Vec2f{static_cast<float>(v.x), static_cast<float>(v.y)};
}

} // namespace pge
