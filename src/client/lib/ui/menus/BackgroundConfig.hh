
#pragma once

#include "olcEngine.hh"

namespace pge {

struct BackgroundConfig
{
  olc::Pixel color{olc::VERY_DARK_GREY};
  olc::Pixel hColor{olc::GREY};
};

auto bgConfigFromColor(const olc::Pixel &bgColor) -> BackgroundConfig;

} // namespace pge
