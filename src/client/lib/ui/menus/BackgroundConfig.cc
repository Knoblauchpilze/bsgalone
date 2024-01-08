
#include "BackgroundConfig.hh"
#include "MenuCommon.hh"

namespace pge {

auto bgConfigFromColor(const olc::Pixel &bgColor) -> BackgroundConfig
{
  return BackgroundConfig{.color  = bgColor,
                          .hColor = colors::toOlcPixel(
                            findHighlightColor(colors::toColor(bgColor)))};
}

} // namespace pge
