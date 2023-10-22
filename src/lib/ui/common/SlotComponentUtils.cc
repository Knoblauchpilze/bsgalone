
#include "SlotComponentUtils.hh"

namespace pge {

auto bgColorFromReloadTime(const bsgo::SlotComponent &component,
                           const float availablePower,
                           const std::optional<float> &distanceToTarget) -> olc::Pixel
{
  auto out = olc::DARK_GREEN;

  if (component.isOffensive() && (!distanceToTarget || component.range() < *distanceToTarget))
  {
    out = olc::DARK_RED;
  }
  else if (availablePower < component.powerCost())
  {
    out = olc::DARK_ORANGE;
  }
  else if (!component.canFire())
  {
    out = colorGradient(olc::DARK_YELLOW,
                        olc::DARK_GREEN,
                        component.reloadPercentage(),
                        alpha::Opaque);
  }

  return out;
}

} // namespace pge
