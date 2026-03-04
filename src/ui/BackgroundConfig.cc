
#include "BackgroundConfig.hh"
#include "MenuCommon.hh"

namespace ui {

auto bgConfigFromColor(const pge::Color &color) -> BackgroundConfig
{
  return BackgroundConfig{.color = color, .hColor = findHighlightColor(color)};
}

} // namespace ui
