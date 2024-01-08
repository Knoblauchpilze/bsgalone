
#include "BackgroundConfig.hh"
#include "MenuCommon.hh"

namespace pge {

auto bgConfigFromColor(const Color &color) -> BackgroundConfig
{
  return BackgroundConfig{.color = color, .hColor = findHighlightColor(color)};
}

} // namespace pge
