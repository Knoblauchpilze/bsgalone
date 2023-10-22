
#pragma once

#include "SlotComponent.hh"
#include "olcEngine.hh"

namespace pge {

auto bgColorFromReloadTime(const bsgo::SlotComponent &component,
                           const float availablePower,
                           const std::optional<float> &distanceToTarget) -> olc::Pixel;

}
