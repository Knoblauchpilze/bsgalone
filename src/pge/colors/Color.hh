
#pragma once

#include "Alpha.hh"
#include "ColorNames.hh"
#include "RgbData.hh"
#include <optional>

namespace pge {

struct Color
{
  std::optional<colors::Name> name{};
  std::optional<RgbData> rgb{};
  uint8_t a{alpha::OPAQUE};
};

} // namespace pge
