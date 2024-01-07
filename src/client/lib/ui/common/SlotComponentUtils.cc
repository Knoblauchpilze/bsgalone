
#include "SlotComponentUtils.hh"

namespace pge {

auto bgColorFromFiringState(const bsgo::SlotComponent &component) -> olc::Pixel
{
  switch (component.firingState())
  {
    case bsgo::FiringState::READY:
      return olc::DARK_GREEN;
    case bsgo::FiringState::DISABLED:
      return olc::DARK_GREY;
    case bsgo::FiringState::INVALID_TARGET:
    case bsgo::FiringState::OUT_OF_RANGE:
      return olc::DARK_RED;
    case bsgo::FiringState::OUT_OF_POWER:
      return toOlcPixel(Color::DARK_ORANGE);
    case bsgo::FiringState::RELOADING:
      return colorGradient(olc::DARK_YELLOW,
                           olc::DARK_GREEN,
                           component.reloadPercentage(),
                           alpha::OPAQUE);
    default:
      return toOlcPixel(Color::PINK);
  }
}

} // namespace pge
