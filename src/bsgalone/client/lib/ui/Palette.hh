
#pragma once

#include "Color.hh"
#include "Faction.hh"

namespace bsgalone::client {

struct Palette
{
  pge::Color activeButtonColor{};
  pge::Color inactiveButtonColor{};

  pge::Color defaultColor{};
  pge::Color almostOpaqueColor{};
};

auto generatePaletteForFaction(const core::Faction faction) -> Palette;

} // namespace bsgalone::client
