
#pragma once

#include "Color.hh"
#include <string>

namespace pge {

enum class TextAlignment
{
  LEFT,
  CENTER,
  RIGHT
};

struct TextConfig
{
  std::string text{""};
  Color color{colors::BLACK};
  Color hColor{colors::WHITE};
  TextAlignment align{TextAlignment::CENTER};
  int margin{0};
};

auto textConfigFromColor(const std::string &text,
                         const Color &color,
                         const TextAlignment &align = TextAlignment::CENTER) -> TextConfig;

auto textConfigFromColor(const std::string &text,
                         const Color &color,
                         const TextAlignment &align,
                         const int margin) -> TextConfig;

auto textConfigFromColor(const std::string &text,
                         const Color &color,
                         const Color &hColor,
                         const TextAlignment &align = TextAlignment::CENTER) -> TextConfig;

auto textConfigFromColor(const std::string &text,
                         const Color &color,
                         const Color &hColor,
                         const TextAlignment &align,
                         const int margin = 0) -> TextConfig;

} // namespace pge
