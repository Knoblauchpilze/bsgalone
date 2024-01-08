
#pragma once

#include "Color.hh"
#include "olcEngine.hh"

namespace pge::colors {

auto toOlcPixel(const Name &name) -> olc::Pixel;
auto toOlcPixel(const Color &color) -> olc::Pixel;

} // namespace pge::colors
