
#include "SlotComponentUtils.hh"

namespace bsgalone::client {

auto bgColorFromFiringState(const core::SlotComponent &component) -> pge::Color
{
  switch (component.firingState())
  {
    case core::FiringState::READY:
      return pge::colors::DARK_GREEN;
    case core::FiringState::DISABLED:
      return bgForMissingComponent();
    case core::FiringState::INVALID_TARGET:
    case core::FiringState::OUT_OF_RANGE:
      return pge::colors::DARK_RED;
    case core::FiringState::OUT_OF_POWER:
      return pge::colors::DARK_ORANGE;
    case core::FiringState::RELOADING:
      return colorGradient(pge::colors::DARK_YELLOW,
                           pge::colors::DARK_GREEN,
                           component.reloadPercentage(),
                           pge::alpha::OPAQUE);
    default:
      return pge::colors::PINK;
  }
}

void resetPictureMenuToDefault(ui::UiPictureMenu &menu, const pge::Color &defaultColor)
{
  menu.setPictureTint(defaultColor);
  menu.updateBgColor(semiOpaque(bgForMissingComponent()));
  menu.setClickCallback({});
  menu.clearSprite();
  menu.setEnabled(false);
}

} // namespace bsgalone::client
