
#pragma once

#include "Vector2d.hh"
#include "olcEngine.hh"

namespace pge {

auto toVf2d(const Vec2f &v) -> olc::vf2d;
auto toVi2d(const Vec2i &v) -> olc::vi2d;

auto toVec2f(const olc::vf2d &v) -> Vec2f;

} // namespace pge