
#pragma once

#include "Item.hh"
#include "PlayerShipRepository.hh"
#include "ShipWeaponRepository.hh"

namespace bsgo {

struct EquipData
{
  Uuid shipId{};

  ShipWeaponRepositoryShPtr shipWeaponRepo{};
  PlayerShipRepositoryShPtr playerShipRepo{};
};

bool canStillEquipWeapon(const EquipData &data);

} // namespace bsgo
