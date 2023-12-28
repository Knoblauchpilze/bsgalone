
#pragma once

#include "ComputerPriceRepository.hh"
#include "Item.hh"
#include "PlayerResourceRepository.hh"
#include "ShipPriceRepository.hh"
#include "Uuid.hh"
#include "WeaponPriceRepository.hh"
#include <memory>
#include <unordered_map>

namespace bsgo {

struct AffordabilityData
{
  Uuid playerId{};
  Uuid itemId{};
  Item itemType{};

  PlayerResourceRepositoryShPtr resourceRepo{};
  WeaponPriceRepositoryShPtr weaponPriceRepo{};
  ComputerPriceRepositoryShPtr computerPriceRepo{};
  ShipPriceRepositoryShPtr shipPriceRepo{};
};

struct Affordability
{
  bool canAfford{false};
  std::unordered_map<Uuid, bool> resourceAvailibility{};
};

auto computeAffordability(const AffordabilityData &data) -> Affordability;

} // namespace bsgo
