
#pragma once

#include "Item.hh"
#include "PlayerShipRepository.hh"
#include "ShipComputerRepository.hh"
#include "ShipWeaponRepository.hh"

namespace bsgalone::core {

struct EquipData
{
  Uuid shipId{};

  ShipWeaponRepositoryShPtr shipWeaponRepo{};
  ShipComputerRepositoryShPtr shipComputerRepo{};
  PlayerShipRepositoryShPtr playerShipRepo{};
};

bool canStillEquipWeapon(const EquipData &data);
bool canStillEquipComputer(const EquipData &data);

} // namespace bsgalone::core
