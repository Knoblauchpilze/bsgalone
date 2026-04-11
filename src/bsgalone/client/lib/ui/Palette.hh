
#pragma once

#include "Color.hh"
#include "Faction.hh"

namespace bsgalone::client {

struct Palette
{
  pge::Color activeButtonColor{};
  pge::Color inactiveButtonColor{};
};

auto generatePaletteForFaction(const core::Faction faction) -> Palette;

} // namespace bsgalone::client
