
#pragma once

#include "Color.hh"
#include "SlotComponent.hh"
#include "UiPictureMenu.hh"

namespace bsgalone::client {

constexpr auto bgForMissingComponent() -> pge::Color;
auto bgColorFromFiringState(const core::SlotComponent &component) -> pge::Color;

void resetPictureMenuToDefault(ui::UiPictureMenu &menu, const pge::Color &defaultColor);

} // namespace bsgalone::client

#include "SlotComponentUtils.hxx"
