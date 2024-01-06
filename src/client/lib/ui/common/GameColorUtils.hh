
#pragma once

#include "Faction.hh"
#include "olcEngine.hh"
#include <string>

namespace pge {

auto colorFromResourceName(const std::string &name) -> olc::Pixel;
auto colorFromFaction(const bsgo::Faction &faction) -> olc::Pixel;

} // namespace pge
