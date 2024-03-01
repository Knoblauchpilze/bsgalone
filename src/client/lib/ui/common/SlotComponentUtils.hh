
#pragma once

#include "Color.hh"
#include "SlotComponent.hh"

namespace pge {

constexpr auto bgForMissingComponent() -> Color;
auto bgColorFromFiringState(const bsgo::SlotComponent &component) -> Color;

} // namespace pge

#include "SlotComponentUtils.hxx"
