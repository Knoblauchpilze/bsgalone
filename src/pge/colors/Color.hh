
#pragma once

#include "Alpha.hh"
#include "ColorNames.hh"
#include "RgbData.hh"
#include <optional>

namespace pge {

struct Color
{
  std::optional<colors::Name> name{colors::Name::WHITE};
  std::optional<RgbData> rgb{};
  uint8_t alpha{alpha::OPAQUE};
};

auto toRgb(const Color &color) -> Color;

} // namespace pge

#include "ColorSpace.hh"
#include "Colors.hh"
#include "Transparency.hh"
