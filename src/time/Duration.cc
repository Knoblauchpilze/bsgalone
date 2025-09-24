
#include "Duration.hh"
#include <stdexcept>

namespace chrono {

auto Duration::str() const -> std::string
{
  return std::to_string(elapsed) + asTimeString(unit);
}

namespace {
constexpr auto MS_IN_A_SECOND = 1000.0f;

auto toMilliseconds(const Unit unit, const float in) -> float
{
  switch (unit)
  {
    case Unit::MILLISECONDS:
      return in;
    case Unit::SECONDS:
      return MS_IN_A_SECOND * in;
    default:
      throw std::invalid_argument("Unsupported time unit " + chrono::str(unit));
  }
}

auto toSeconds(const Unit unit, const float in) -> float
{
  switch (unit)
  {
    case Unit::MILLISECONDS:
      return in / MS_IN_A_SECOND;
    case Unit::SECONDS:
      return in;
    default:
      throw std::invalid_argument("Unsupported time unit " + chrono::str(unit));
  }
}
} // namespace

auto Duration::convert(const Unit out) const -> Duration
{
  float value{0.0f};

  switch (out)
  {
    case Unit::MILLISECONDS:
      value = toMilliseconds(unit, elapsed);
      break;
    case Unit::SECONDS:
      value = toSeconds(unit, elapsed);
      break;
    default:
      throw std::invalid_argument("Unsupported time unit " + chrono::str(out));
  }

  return Duration{.unit = out, .elapsed = value};
}

} // namespace chrono
