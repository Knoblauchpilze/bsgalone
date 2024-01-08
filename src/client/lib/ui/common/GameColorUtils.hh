
#pragma once

#include "Color.hh"
#include "Faction.hh"
#include "olcEngine.hh"
#include <string>

namespace pge {

auto colorFromResourceName(const std::string &name) -> Color;
auto colorFromFaction(const bsgo::Faction &faction) -> olc::Pixel;

} // namespace pge
