
#include "LockerService.hh"
#include "LockerUtils.hh"

namespace bsgo {

LockerService::LockerService(const Repositories &repositories)
  : AbstractService("locker", repositories)
{}

bool LockerService::tryEquip(const LockerItemData &item) const
{
  const auto ship = m_repositories.playerShipRepository->findOneById(item.shipDbId);

  if (!verifySlotAvailability(item.shipDbId, item.type))
  {
    warn("Failed to equip item " + str(item.dbId) + " on ship " + str(item.shipDbId),
         "Ship does not have any available slot anymore");
    return false;
  }
  if (!verifyItemIsNotEquiped(item.dbId, item.type))
  {
    warn("Failed to equip item " + str(item.dbId) + " on ship " + str(item.shipDbId),
         "Item is already equiped");
    return false;
  }
  if (!verifyItemBelongsToPlayer(ship.player, item.dbId, item.type))
  {
    warn("Failed to equip item " + str(item.dbId) + " on ship " + str(item.shipDbId),
         "Item does not belong to player");
    return false;
  }

  switch (item.type)
  {
    case bsgalone::core::Item::WEAPON:
      tryEquipWeapon(item.shipDbId, item.dbId);
      break;
    case bsgalone::core::Item::COMPUTER:
      tryEquipComputer(item.shipDbId, item.dbId);
      break;
    default:
      error("Invalid kind of item to equip", "Unsupported item " + str(item.type));
      break;
  }

  return true;
}

bool LockerService::tryUnequip(const LockerItemData &item) const
{
  const auto ship = m_repositories.playerShipRepository->findOneById(item.shipDbId);

  if (!verifyItemIsEquiped(item.shipDbId, item.dbId, item.type))
  {
    warn("Failed to unequip item " + str(item.dbId) + " on ship " + str(item.shipDbId),
         "Item is not equiped");
    return false;
  }
  if (!verifyItemBelongsToPlayer(ship.player, item.dbId, item.type))
  {
    warn("Failed to unequip item " + str(item.dbId) + " on ship " + str(item.shipDbId),
         "Item does not belong to player");
    return false;
  }

  switch (item.type)
  {
    case bsgalone::core::Item::WEAPON:
      tryUnequipWeapon(item.shipDbId, item.dbId);
      break;
    case bsgalone::core::Item::COMPUTER:
      tryUnequipComputer(item.shipDbId, item.dbId);
      break;
    default:
      error("Invalid kind of item to unequip", "Unsupported item " + str(item.type));
      break;
  }

  return true;
}

bool LockerService::verifySlotAvailability(const Uuid shipDbId,
                                           const bsgalone::core::Item &type) const
{
  EquipData data{.shipId           = shipDbId,
                 .shipWeaponRepo   = m_repositories.shipWeaponRepository,
                 .shipComputerRepo = m_repositories.shipComputerRepository,
                 .playerShipRepo   = m_repositories.playerShipRepository};

  switch (type)
  {
    case bsgalone::core::Item::WEAPON:
      return canStillEquipWeapon(data);
    case bsgalone::core::Item::COMPUTER:
      return canStillEquipComputer(data);
    default:
      error("Failed to verify slot availability", "Unsupported item " + str(type));
  }

  // Redundant because of the error above.
  return false;
}

bool LockerService::verifyItemBelongsToPlayer(const Uuid playerDbId,
                                              const Uuid itemId,
                                              const bsgalone::core::Item &type) const
{
  bool belong{false};
  switch (type)
  {
    case bsgalone::core::Item::WEAPON:
      belong = m_repositories.playerWeaponRepository->findAllByPlayer(playerDbId).contains(itemId);
      break;
    case bsgalone::core::Item::COMPUTER:
      belong = m_repositories.playerComputerRepository->findAllByPlayer(playerDbId).contains(itemId);
      break;
    default:
      error("Failed to verify if item belongs to player", "Unsupported item " + str(type));
  }

  return belong;
}

bool LockerService::verifyItemIsEquiped(const Uuid shipDbId,
                                        const Uuid itemId,
                                        const bsgalone::core::Item &type) const
{
  bool equiped{false};

  switch (type)
  {
    case bsgalone::core::Item::WEAPON:
      equiped = m_repositories.shipWeaponRepository->findOneByShipAndWeapon(shipDbId, itemId)
                  .has_value();
      break;
    case bsgalone::core::Item::COMPUTER:
      equiped = m_repositories.shipComputerRepository->existByShipAndComputer(shipDbId, itemId);
      break;
    default:
      error("Failed to verify item is equiped", "Unsupported item " + str(type));
  }

  return equiped;
}

bool LockerService::verifyItemIsNotEquiped(const Uuid itemId, const bsgalone::core::Item &type) const
{
  bool equiped{true};

  switch (type)
  {
    case bsgalone::core::Item::WEAPON:
      equiped = m_repositories.shipWeaponRepository->findOneByWeapon(itemId).has_value();
      break;
    case bsgalone::core::Item::COMPUTER:
      equiped = m_repositories.shipComputerRepository->existByComputer(itemId);
      break;
    default:
      error("Failed to verify item is not equiped", "Unsupported item " + str(type));
  }

  return !equiped;
}

void LockerService::tryEquipWeapon(const Uuid shipDbId, const Uuid weaponId) const
{
  const auto slots = m_repositories.playerShipRepository->findAllAvailableWeaponSlotByShip(shipDbId);
  if (slots.empty())
  {
    error("Failed to find available weapon slots");
  }
  const auto slot = *slots.begin();

  ShipWeapon weapon{.ship = shipDbId, .weapon = weaponId, .slot = slot};
  debug("Installing weapon " + str(weaponId) + " in slot " + str(slot));
  m_repositories.shipWeaponRepository->save(weapon);
}

void LockerService::tryEquipComputer(const Uuid shipDbId, const Uuid computerId) const
{
  debug("Installing computer " + str(computerId) + " for ship " + str(shipDbId));
  m_repositories.shipComputerRepository->save(shipDbId, computerId);
}

void LockerService::tryUnequipWeapon(const Uuid shipDbId, const Uuid weaponId) const
{
  const auto data = m_repositories.shipWeaponRepository->findOneByShipAndWeapon(shipDbId, weaponId);
  debug("Uninstalling weapon " + str(data->weapon) + " from slot " + str(data->slot));
  m_repositories.shipWeaponRepository->deleteByShipAndSlot(*data);
}

void LockerService::tryUnequipComputer(const Uuid shipDbId, const Uuid computerId) const
{
  debug("Uninstalling computer " + str(computerId) + " from ship " + str(shipDbId));
  m_repositories.shipComputerRepository->deleteByShipAndId(shipDbId, computerId);
}

} // namespace bsgo
