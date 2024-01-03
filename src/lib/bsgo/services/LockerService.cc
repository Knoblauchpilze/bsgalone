
#include "LockerService.hh"
#include "LockerUtils.hh"

namespace bsgo {

LockerService::LockerService(const Repositories &repositories, const CoordinatorShPtr &coordinator)
  : AbstractService("locker", repositories, coordinator)
{}

bool LockerService::tryEquip(const LockerItemData &item) const
{
  const auto ship = m_repositories.playerShipRepository->findOneById(item.shipDbId);

  if (!ship.player)
  {
    return false;
  }
  if (!verifySlotAvailability(item.shipDbId, item.type))
  {
    return false;
  }
  if (!verifyItemIsNotEquiped(item.dbId, item.type))
  {
    log("haha");
    return false;
  }
  if (!verifyItemBelongsToPlayer(*ship.player, item.dbId, item.type))
  {
    return false;
  }

  switch (item.type)
  {
    case Item::WEAPON:
      tryEquipWeapon(item.shipDbId, item.dbId);
      break;
    case Item::COMPUTER:
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

  if (!ship.player)
  {
    return false;
  }
  if (!verifyItemIsEquiped(item.shipDbId, item.dbId, item.type))
  {
    return false;
  }
  if (!verifyItemBelongsToPlayer(*ship.player, item.dbId, item.type))
  {
    return false;
  }

  switch (item.type)
  {
    case Item::WEAPON:
      tryUnequipWeapon(item.shipDbId, item.dbId);
      break;
    case Item::COMPUTER:
      tryUnequipComputer(item.shipDbId, item.dbId);
      break;
    default:
      error("Invalid kind of item to unequip", "Unsupported item " + str(item.type));
      break;
  }

  return true;
}

bool LockerService::verifySlotAvailability(const Uuid &shipDbId, const Item &type) const
{
  EquipData data{.shipId           = shipDbId,
                 .shipWeaponRepo   = m_repositories.shipWeaponRepository,
                 .shipComputerRepo = m_repositories.shipComputerRepository,
                 .playerShipRepo   = m_repositories.playerShipRepository};

  switch (type)
  {
    case Item::WEAPON:
      return canStillEquipWeapon(data);
    case Item::COMPUTER:
      return canStillEquipComputer(data);
    default:
      error("Failed to verify slot availability", "Unsupported item " + str(type));
  }

  // Redundant because of the error above.
  return false;
}

bool LockerService::verifyItemBelongsToPlayer(const Uuid &playerDbId,
                                              const Uuid &itemId,
                                              const Item &type) const
{
  bool belong{false};
  switch (type)
  {
    case Item::WEAPON:
      belong = m_repositories.playerWeaponRepository->findAllByPlayer(playerDbId).contains(itemId);
      break;
    case Item::COMPUTER:
      belong = m_repositories.playerComputerRepository->findAllByPlayer(playerDbId).contains(itemId);
      break;
    default:
      error("Failed to verify if item belongs to player", "Unsupported item " + str(type));
  }

  return belong;
}

bool LockerService::verifyItemIsEquiped(const Uuid &shipDbId,
                                        const Uuid &itemId,
                                        const Item &type) const
{
  bool equiped{false};

  switch (type)
  {
    case Item::WEAPON:
      equiped = m_repositories.shipWeaponRepository->findOneByShipAndWeapon(shipDbId, itemId)
                  .has_value();
      break;
    case Item::COMPUTER:
      equiped = m_repositories.shipComputerRepository->existByShipAndComputer(shipDbId, itemId);
      break;
    default:
      error("Failed to verify item is equiped", "Unsupported item " + str(type));
  }

  return equiped;
}

bool LockerService::verifyItemIsNotEquiped(const Uuid &itemId, const Item &type) const
{
  bool equiped{true};

  switch (type)
  {
    case Item::WEAPON:
      equiped = m_repositories.shipWeaponRepository->findOneByWeapon(itemId).has_value();
      break;
    case Item::COMPUTER:
      equiped = m_repositories.shipComputerRepository->existByComputer(itemId);
      break;
    default:
      error("Failed to verify item is not equiped", "Unsupported item " + str(type));
  }

  return !equiped;
}

void LockerService::tryEquipWeapon(const Uuid &shipDbId, const Uuid &weaponId) const
{
  const auto slots = m_repositories.playerShipRepository->findAllAvailableWeaponSlotByShip(shipDbId);
  if (slots.empty())
  {
    error("Failed to find available weapon slots");
  }
  const auto slot = *slots.begin();

  ShipWeapon weapon{.ship = shipDbId, .weapon = weaponId, .slot = slot};
  log("Installing weapon " + str(weaponId) + " in slot " + str(slot));
  m_repositories.shipWeaponRepository->save(weapon);
}

void LockerService::tryEquipComputer(const Uuid &shipDbId, const Uuid &computerId) const
{
  log("Installing computer " + str(computerId) + " for ship " + str(shipDbId));
  m_repositories.shipComputerRepository->save(shipDbId, computerId);
}

void LockerService::tryUnequipWeapon(const Uuid &shipDbId, const Uuid &weaponId) const
{
  const auto data = m_repositories.shipWeaponRepository->findOneByShipAndWeapon(shipDbId, weaponId);
  log("Uninstalling weapon " + str(data->weapon) + " from slot " + str(data->slot));
  m_repositories.shipWeaponRepository->deleteByShipAndSlot(*data);
}

void LockerService::tryUnequipComputer(const Uuid &shipDbId, const Uuid &computerId) const
{
  log("Uninstalling computer " + str(computerId) + " from ship " + str(shipDbId));
  m_repositories.shipComputerRepository->deleteByShipAndId(shipDbId, computerId);
}

} // namespace bsgo
