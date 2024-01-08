
#pragma once

#include "Color.hh"
#include "olcPixelGameEngine.h"

namespace pge::colors {

auto toOlcPixel(const Name &name) -> olc::Pixel;
auto toOlcPixel(const Color &color) -> olc::Pixel;

auto toColor(const olc::Pixel &color) -> Color;

} // namespace pge::colors
