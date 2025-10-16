
#include "Unit.hh"
#include <stdexcept>

namespace chrono {

auto str(const Unit unit) -> std::string
{
  switch (unit)
  {
    case Unit::MILLISECONDS:
      return "milliseconds";
    case Unit::SECONDS:
      return "seconds";
    default:
      return "unknown";
  }
}

auto asTimeString(const Unit unit) -> std::string
{
  switch (unit)
  {
    case Unit::MILLISECONDS:
      return "ms";
    case Unit::SECONDS:
      return "s";
    default:
      return "N/A";
  }
}

auto fromString(const std::string_view unit) -> Unit
{
  if (unit == "milliseconds")
  {
    return Unit::MILLISECONDS;
  }

  if (unit == "seconds")
  {
    return Unit::SECONDS;
  }

  throw std::invalid_argument(std::string("Unsupported time unit ") + std::string(unit));
}

} // namespace chrono
