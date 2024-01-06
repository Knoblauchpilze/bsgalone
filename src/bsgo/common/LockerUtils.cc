
#include "LockerUtils.hh"
#include "Slot.hh"

namespace bsgo {

bool canStillEquipWeapon(const EquipData &data)
{
  const auto weapons = data.shipWeaponRepo->findAllByShip(data.shipId);
  const auto slots   = data.playerShipRepo->findOneById(data.shipId).slots;

  const auto alreadyEquiped = static_cast<int>(weapons.size());
  return alreadyEquiped < slots.at(Slot::WEAPON);
}

bool canStillEquipComputer(const EquipData &data)
{
  const auto computers = data.shipComputerRepo->findAllByShip(data.shipId);
  const auto slots     = data.playerShipRepo->findOneById(data.shipId).slots;

  const auto alreadyEquiped = static_cast<int>(computers.size());
  return alreadyEquiped < slots.at(Slot::COMPUTER);
}

} // namespace bsgo
