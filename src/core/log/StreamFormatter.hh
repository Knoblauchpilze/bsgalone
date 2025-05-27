
#pragma once

#include "Severity.hh"
#include <ostream>
#include <string>

namespace core::log {

enum class Color
{
  BLUE,
  CYAN,
  GREEN,
  GREY,
  MAGENTA,
  RED,
  YELLOW
};

void setStreamColorFromSeverity(std::ostream &stream, const Severity severity);
void setStreamColor(std::ostream &stream, const Color &color);
void clearStreamFormat(std::ostream &stream);

} // namespace core::log
