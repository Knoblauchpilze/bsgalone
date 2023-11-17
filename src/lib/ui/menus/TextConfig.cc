
#include "TextConfig.hh"
#include "MenuCommon.hh"

namespace pge {

auto textConfigFromColor(const std::string &text,
                         const olc::Pixel &color,
                         const TextAlignment &align) -> TextConfig
{
  return TextConfig{.text   = text,
                    .color  = color,
                    .hColor = findHighlightColor(color),
                    .align  = align};
}

} // namespace pge
