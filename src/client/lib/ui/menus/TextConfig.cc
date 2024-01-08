
#include "TextConfig.hh"
#include "MenuCommon.hh"

namespace pge {

auto textConfigFromColor(const std::string &text,
                         const olc::Pixel &color,
                         const TextAlignment &align) -> TextConfig
{
  const auto in = colors::toColor(color);
  return textConfigFromColor(text, in, findHighlightColor(in), align);
}

auto textConfigFromColor(const std::string &text,
                         const Color &color,
                         const Color &hColor,
                         const TextAlignment &align) -> TextConfig
{
  return TextConfig{.text = text, .color = color, .hColor = hColor, .align = align};
}

} // namespace pge
