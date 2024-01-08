
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
      return colors::toOlcPixel(colors::Name::DARK_ORANGE);
    case bsgo::FiringState::RELOADING:
      return colors::toOlcPixel(colorGradient(colors::DARK_YELLOW,
                                              colors::DARK_GREEN,
                                              component.reloadPercentage(),
                                              alpha::OPAQUE));
    default:
      return colors::toOlcPixel(colors::Name::PINK);
  }
}

} // namespace pge
