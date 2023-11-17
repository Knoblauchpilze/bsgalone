
#pragma once

#include "olcEngine.hh"
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
  olc::Pixel color{olc::BLACK};
  olc::Pixel hColor{olc::WHITE};
  TextAlignment align{TextAlignment::CENTER};
};

auto textConfigFromColor(const std::string &text,
                         const olc::Pixel &color,
                         const TextAlignment &align = TextAlignment::CENTER) -> TextConfig;

} // namespace pge
