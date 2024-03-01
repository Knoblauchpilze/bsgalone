
#pragma once

#include "Color.hh"
#include "SlotComponent.hh"
#include "UiPictureMenu.hh"

namespace pge {

constexpr auto bgForMissingComponent() -> Color;
auto bgColorFromFiringState(const bsgo::SlotComponent &component) -> Color;

void resetPictureMenuToDefault(UiPictureMenu &menu, const Color &defaultColor);

} // namespace pge

#include "SlotComponentUtils.hxx"
