
#include "TextConfig.hh"
#include "MenuCommon.hh"

namespace pge {

auto textConfigFromColor(const std::string &text, const Color &color, const TextAlignment &align)
  -> TextConfig
{
  return textConfigFromColor(text, color, findHighlightColor(color), align);
}

auto textConfigFromColor(const std::string &text,
                         const Color &color,
                         const Color &hColor,
                         const TextAlignment &align) -> TextConfig
{
  return TextConfig{.text = text, .color = color, .hColor = hColor, .align = align};
}

} // namespace pge
