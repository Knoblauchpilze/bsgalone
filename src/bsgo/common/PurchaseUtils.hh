
#pragma once

#include "ComputerData.hh"
#include "Item.hh"
#include "PlayerResourceData.hh"
#include "ShipData.hh"
#include "Uuid.hh"
#include "WeaponData.hh"
#include <memory>
#include <unordered_map>
#include <vector>

namespace bsgo {

struct AffordabilityData
{
  Uuid playerId{};
  Uuid itemId{};
  Item itemType{};

  std::vector<PlayerResourceData> playerResources{};
  std::vector<WeaponData> weapons{};
  std::vector<ComputerData> computers{};
  std::vector<ShipData> ships{};
};

struct Affordability
{
  bool canAfford{false};
  std::unordered_map<Uuid, bool> resourceAvailibility{};
};

auto computeAffordability(const AffordabilityData &data) -> Affordability;

} // namespace bsgo
