
#pragma once

#include "Color.hh"
#include "Faction.hh"
#include <string>

namespace bsgalone::client {

auto colorFromResourceName(const std::string &name) -> pge::Color;
auto colorFromFaction(const core::Faction faction) -> pge::Color;

struct ColorPalette
{
  pge::Color defaultColor{};
  pge::Color almostOpaqueColor{};
};

auto generatePaletteForFaction(const core::Faction faction) -> ColorPalette;

} // namespace bsgalone::client
