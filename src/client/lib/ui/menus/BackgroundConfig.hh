
#pragma once

#include "Color.hh"

namespace pge {

struct BackgroundConfig
{
  Color color{colors::VERY_DARK_GREY};
  Color hColor{colors::GREY};
};

auto bgConfigFromColor(const Color &color) -> BackgroundConfig;

} // namespace pge
