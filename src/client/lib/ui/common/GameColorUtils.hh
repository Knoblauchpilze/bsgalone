
#pragma once

#include "Color.hh"
#include "Faction.hh"
#include "PlayerView.hh"
#include <string>

namespace pge {

auto colorFromResourceName(const std::string &name) -> Color;
auto colorFromFaction(const bsgo::Faction faction) -> Color;

struct ColorPalette
{
  Color defaultColor{};
  Color almostOpaqueColor{};
};

auto generatePaletteForFaction(const bsgo::Faction faction) -> ColorPalette;

} // namespace pge
