
#pragma once

#include "Color.hh"
#include "Colors.hh"
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
};

auto textConfigFromColor(const std::string &text,
                         const Color &color,
                         const TextAlignment &align = TextAlignment::CENTER) -> TextConfig;

auto textConfigFromColor(const std::string &text,
                         const Color &color,
                         const Color &hColor,
                         const TextAlignment &align = TextAlignment::CENTER) -> TextConfig;

} // namespace pge
