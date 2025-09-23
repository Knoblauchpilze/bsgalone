
#include "Unit.hh"

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

} // namespace chrono
