
#pragma once

#include "Color.hh"
#include <string>

namespace ui {

enum class TextAlignment
{
  LEFT,
  CENTER,
  RIGHT
};

struct TextConfig
{
  std::string text{""};
  pge::Color color{pge::colors::BLACK};
  pge::Color hColor{pge::colors::WHITE};
  TextAlignment align{TextAlignment::CENTER};
  int margin{0};
};

auto textConfigFromColor(const std::string &text,
                         const pge::Color &color,
                         const TextAlignment &align = TextAlignment::CENTER) -> TextConfig;

auto textConfigFromColor(const std::string &text,
                         const pge::Color &color,
                         const TextAlignment &align,
                         const int margin) -> TextConfig;

auto textConfigFromColor(const std::string &text,
                         const pge::Color &color,
                         const pge::Color &hColor,
                         const TextAlignment &align = TextAlignment::CENTER) -> TextConfig;

auto textConfigFromColor(const std::string &text,
                         const pge::Color &color,
                         const pge::Color &hColor,
                         const TextAlignment &align,
                         const int margin = 0) -> TextConfig;

} // namespace ui
