
#include "SlotComponentUtils.hh"

namespace pge {

auto bgColorFromFiringState(const bsgo::SlotComponent &component) -> Color
{
  switch (component.firingState())
  {
    case bsgo::FiringState::READY:
      return colors::DARK_GREEN;
    case bsgo::FiringState::DISABLED:
      return colors::DARK_GREY;
    case bsgo::FiringState::INVALID_TARGET:
    case bsgo::FiringState::OUT_OF_RANGE:
      return colors::DARK_RED;
    case bsgo::FiringState::OUT_OF_POWER:
      return colors::DARK_ORANGE;
    case bsgo::FiringState::RELOADING:
      return colorGradient(colors::DARK_YELLOW,
                           colors::DARK_GREEN,
                           component.reloadPercentage(),
                           alpha::OPAQUE);
    default:
      return colors::PINK;
  }
}

} // namespace pge
