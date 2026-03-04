
#pragma once

#include "Color.hh"

namespace ui {

struct BackgroundConfig
{
  pge::Color color{pge::colors::VERY_DARK_GREY};
  pge::Color hColor{pge::colors::GREY};
};

auto bgConfigFromColor(const pge::Color &color) -> BackgroundConfig;

} // namespace ui
