
#include "LockerService.hh"
#include "LockerUtils.hh"

namespace bsgalone::server {

LockerService::LockerService(const core::Repositories &repositories)
  : AbstractService("locker", repositories)
{}

bool LockerService::tryEquip(const LockerItemData &item) const
{
  const auto ship = m_repositories.playerShipRepository->findOneById(item.shipDbId);

  if (!verifySlotAvailability(item.shipDbId, item.type))
  {
    warn("Failed to equip item " + core::str(item.dbId) + " on ship " + core::str(item.shipDbId),
         "Ship does not have any available slot anymore");
    return false;
  }
  if (!verifyItemIsNotEquiped(item.dbId, item.type))
  {
    warn("Failed to equip item " + core::str(item.dbId) + " on ship " + core::str(item.shipDbId),
         "Item is already equiped");
    return false;
  }
  if (!verifyItemBelongsToPlayer(ship.player, item.dbId, item.type))
  {
    warn("Failed to equip item " + core::str(item.dbId) + " on ship " + core::str(item.shipDbId),
         "Item does not belong to player");
    return false;
  }

  switch (item.type)
  {
    case core::Item::WEAPON:
      tryEquipWeapon(item.shipDbId, item.dbId);
      break;
    case core::Item::COMPUTER:
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
    warn("Failed to unequip item " + core::str(item.dbId) + " on ship " + core::str(item.shipDbId),
         "Item is not equiped");
    return false;
  }
  if (!verifyItemBelongsToPlayer(ship.player, item.dbId, item.type))
  {
    warn("Failed to unequip item " + core::str(item.dbId) + " on ship " + core::str(item.shipDbId),
         "Item does not belong to player");
    return false;
  }

  switch (item.type)
  {
    case core::Item::WEAPON:
      tryUnequipWeapon(item.shipDbId, item.dbId);
      break;
    case core::Item::COMPUTER:
      tryUnequipComputer(item.shipDbId, item.dbId);
      break;
    default:
      error("Invalid kind of item to unequip", "Unsupported item " + str(item.type));
      break;
  }

  return true;
}

bool LockerService::verifySlotAvailability(const core::Uuid shipDbId, const core::Item &type) const
{
  core::EquipData data{.shipId           = shipDbId,
                       .shipWeaponRepo   = m_repositories.shipWeaponRepository,
                       .shipComputerRepo = m_repositories.shipComputerRepository,
                       .playerShipRepo   = m_repositories.playerShipRepository};

  switch (type)
  {
    case core::Item::WEAPON:
      return core::canStillEquipWeapon(data);
    case core::Item::COMPUTER:
      return core::canStillEquipComputer(data);
    default:
      error("Failed to verify slot availability", "Unsupported item " + str(type));
  }

  // Redundant because of the error above.
  return false;
}

bool LockerService::verifyItemBelongsToPlayer(const core::Uuid playerDbId,
                                              const core::Uuid itemId,
                                              const core::Item &type) const
{
  bool belong{false};
  switch (type)
  {
    case core::Item::WEAPON:
      belong = m_repositories.playerWeaponRepository->findAllByPlayer(playerDbId).contains(itemId);
      break;
    case core::Item::COMPUTER:
      belong = m_repositories.playerComputerRepository->findAllByPlayer(playerDbId).contains(itemId);
      break;
    default:
      error("Failed to verify if item belongs to player", "Unsupported item " + str(type));
  }

  return belong;
}

bool LockerService::verifyItemIsEquiped(const core::Uuid shipDbId,
                                        const core::Uuid itemId,
                                        const core::Item &type) const
{
  bool equiped{false};

  switch (type)
  {
    case core::Item::WEAPON:
      equiped = m_repositories.shipWeaponRepository->findOneByShipAndWeapon(shipDbId, itemId)
                  .has_value();
      break;
    case core::Item::COMPUTER:
      equiped = m_repositories.shipComputerRepository->existByShipAndComputer(shipDbId, itemId);
      break;
    default:
      error("Failed to verify item is equiped", "Unsupported item " + str(type));
  }

  return equiped;
}

bool LockerService::verifyItemIsNotEquiped(const core::Uuid itemId, const core::Item &type) const
{
  bool equiped{true};

  switch (type)
  {
    case core::Item::WEAPON:
      equiped = m_repositories.shipWeaponRepository->findOneByWeapon(itemId).has_value();
      break;
    case core::Item::COMPUTER:
      equiped = m_repositories.shipComputerRepository->existByComputer(itemId);
      break;
    default:
      error("Failed to verify item is not equiped", "Unsupported item " + str(type));
  }

  return !equiped;
}

void LockerService::tryEquipWeapon(const core::Uuid shipDbId, const core::Uuid weaponId) const
{
  const auto slots = m_repositories.playerShipRepository->findAllAvailableWeaponSlotByShip(shipDbId);
  if (slots.empty())
  {
    error("Failed to find available weapon slots");
  }
  const auto slot = *slots.begin();

  core::ShipWeapon weapon{.ship = shipDbId, .weapon = weaponId, .slot = slot};
  debug("Installing weapon " + core::str(weaponId) + " in slot " + core::str(slot));
  m_repositories.shipWeaponRepository->save(weapon);
}

void LockerService::tryEquipComputer(const core::Uuid shipDbId, const core::Uuid computerId) const
{
  debug("Installing computer " + core::str(computerId) + " for ship " + core::str(shipDbId));
  m_repositories.shipComputerRepository->save(shipDbId, computerId);
}

void LockerService::tryUnequipWeapon(const core::Uuid shipDbId, const core::Uuid weaponId) const
{
  const auto data = m_repositories.shipWeaponRepository->findOneByShipAndWeapon(shipDbId, weaponId);
  debug("Uninstalling weapon " + core::str(data->weapon) + " from slot " + core::str(data->slot));
  m_repositories.shipWeaponRepository->deleteByShipAndSlot(*data);
}

void LockerService::tryUnequipComputer(const core::Uuid shipDbId, const core::Uuid computerId) const
{
  debug("Uninstalling computer " + core::str(computerId) + " from ship " + core::str(shipDbId));
  m_repositories.shipComputerRepository->deleteByShipAndId(shipDbId, computerId);
}

} // namespace bsgalone::server
