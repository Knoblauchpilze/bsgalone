
#include "StreamFormatter.hh"
#include "Severity.hh"

namespace core::log {
namespace {
constexpr auto STREAM_FORMAT_CLEAR = "\033[0m";

constexpr auto STREAM_BLUE_FORMATTER    = "\033[1;34m";
constexpr auto STREAM_GREEN_FORMATTER   = "\033[1;32m";
constexpr auto STREAM_GREY_FORMATTER    = "\033[1;90m";
constexpr auto STREAM_MAGENTA_FORMATTER = "\033[1;35m";
constexpr auto STREAM_CYAN_FORMATTER    = "\033[1;36m";
constexpr auto STREAM_RED_FORMATTER     = "\033[1;31m";
constexpr auto STREAM_YELLOW_FORMATTER  = "\033[1;33m";

auto formatterFromColor(const Color &color) -> const char *
{
  switch (color)
  {
    case Color::BLUE:
      return STREAM_BLUE_FORMATTER;
    case Color::CYAN:
      return STREAM_CYAN_FORMATTER;
    case Color::GREEN:
      return STREAM_GREEN_FORMATTER;
    case Color::MAGENTA:
      return STREAM_MAGENTA_FORMATTER;
    case Color::RED:
      return STREAM_RED_FORMATTER;
    case Color::YELLOW:
      return STREAM_YELLOW_FORMATTER;
    case Color::GREY:
    default:
      return STREAM_GREY_FORMATTER;
  }
}

auto colorFromSeverity(const Severity severity) -> Color
{
  switch (severity)
  {
    case Severity::ERROR:
      return Color::RED;
    case Severity::WARNING:
      return Color::YELLOW;
    case Severity::NOTICE:
      return Color::CYAN;
    case Severity::INFO:
      return Color::GREEN;
    case Severity::DEBUG:
      return Color::BLUE;
    case Severity::VERBOSE:
    default:
      return Color::GREY;
  }
}
} // namespace

void setStreamColorFromSeverity(std::ostream &stream, const Severity severity)
{
  setStreamColor(stream, colorFromSeverity(severity));
}

void setStreamColor(std::ostream &stream, const Color &color)
{
  stream << formatterFromColor(color);
}

void clearStreamFormat(std::ostream &stream)
{
  stream << STREAM_FORMAT_CLEAR;
}

} // namespace core::log
