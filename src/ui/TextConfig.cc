
#include "TextConfig.hh"
#include "MenuCommon.hh"

namespace ui {

auto textConfigFromColor(const std::string &text,
                         const pge::Color &color,
                         const TextAlignment &align) -> TextConfig
{
  return textConfigFromColor(text, color, findHighlightColor(color), align, 0);
}

auto textConfigFromColor(const std::string &text,
                         const pge::Color &color,
                         const TextAlignment &align,
                         const int margin) -> TextConfig
{
  return textConfigFromColor(text, color, findHighlightColor(color), align, margin);
}

auto textConfigFromColor(const std::string &text,
                         const pge::Color &color,
                         const pge::Color &hColor,
                         const TextAlignment &align) -> TextConfig
{
  return textConfigFromColor(text, color, hColor, align, 0);
}

auto textConfigFromColor(const std::string &text,
                         const pge::Color &color,
                         const pge::Color &hColor,
                         const TextAlignment &align,
                         const int margin) -> TextConfig
{
  return TextConfig{.text = text, .color = color, .hColor = hColor, .align = align, .margin = margin};
}

} // namespace ui
