
#include "LockerService.hh"
#include "LockerUtils.hh"

namespace bsgo {

LockerService::LockerService(const Repositories &repositories)
  : AbstractService("locker", repositories)
{}

void LockerService::setPlayerDbId(const Uuid &player)
{
  m_playerDbId = player;
}

void LockerService::setPlayerShipDbId(const Uuid &ship)
{
  m_playerShipDbId = ship;
}

bool LockerService::isReady() const noexcept
{
  return m_playerDbId.has_value() && m_playerShipDbId.has_value();
}

bool LockerService::tryEquip(const Uuid &id, const Item &type) const
{
  checkPlayerDbIdExists();
  checkPlayerShipDbIdExists();

  if (!verifySlotAvailability(type))
  {
    return false;
  }
  if (!verifyItemBelongsToPlayer(id, type))
  {
    return false;
  }

  switch (type)
  {
    case Item::WEAPON:
      tryEquipWeapon(id);
      break;
    case Item::COMPUTER:
      tryEquipComputer(id);
      break;
    default:
      error("Invalid kind of item to equip", "Unsupported item " + str(type));
      break;
  }

  return true;
}

bool LockerService::tryUnequip(const Uuid &id, const Item &type) const
{
  checkPlayerDbIdExists();
  checkPlayerShipDbIdExists();

  if (!verifyItemIsEquiped(id, type))
  {
    return false;
  }
  if (!verifyItemBelongsToPlayer(id, type))
  {
    return false;
  }

  switch (type)
  {
    case Item::WEAPON:
      tryUnequipWeapon(id);
      break;
    case Item::COMPUTER:
      tryUnequipComputer(id);
      break;
    default:
      error("Invalid kind of item to unequip", "Unsupported item " + str(type));
      break;
  }

  return true;
}

void LockerService::checkPlayerDbIdExists() const
{
  if (!m_playerDbId)
  {
    error("Expected player db id to exist but it does not");
  }
}

void LockerService::checkPlayerShipDbIdExists() const
{
  if (!m_playerShipDbId)
  {
    error("Expected player ship db id to exist but it does not");
  }
}

bool LockerService::verifySlotAvailability(const Item &type) const
{
  EquipData data{.shipId           = *m_playerShipDbId,
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

bool LockerService::verifyItemBelongsToPlayer(const Uuid &item, const Item &type) const
{
  bool belong{false};
  switch (type)
  {
    case Item::WEAPON:
      belong = m_repositories.playerWeaponRepository->findAllByPlayer(*m_playerDbId).contains(item);
      break;
    case Item::COMPUTER:
      belong = m_repositories.playerComputerRepository->findAllByPlayer(*m_playerDbId).contains(item);
      break;
    default:
      error("Failed to verify if item belongs to player", "Unsupported item " + str(type));
  }

  return belong;
}

bool LockerService::verifyItemIsEquiped(const Uuid &item, const Item &type) const
{
  bool equiped{false};

  switch (type)
  {
    case Item::WEAPON:
      equiped = m_repositories.shipWeaponRepository->findOneByShipAndWeapon(*m_playerShipDbId, item)
                  .has_value();
      break;
    case Item::COMPUTER:
      equiped = m_repositories.shipComputerRepository->existByShipAndComputer(*m_playerShipDbId,
                                                                              item);
      break;
    default:
      error("Failed to verify item is equiped", "Unsupported item " + str(type));
  }

  return equiped;
}

void LockerService::tryEquipWeapon(const Uuid &id) const
{
  const auto slots = m_repositories.playerShipRepository->findAllAvailableWeaponSlotByShip(
    *m_playerShipDbId);
  if (slots.empty())
  {
    error("Failed to find available weapon slots");
  }
  const auto slot = *slots.begin();

  ShipWeapon weapon{.ship = *m_playerShipDbId, .weapon = id, .slot = slot};
  log("Installing weapon " + str(id) + " in slot " + str(slot));
  m_repositories.shipWeaponRepository->save(weapon);
}

void LockerService::tryEquipComputer(const Uuid &id) const
{
  log("Installing computer " + str(id) + " for ship " + str(*m_playerShipDbId));
  m_repositories.shipComputerRepository->save(*m_playerShipDbId, id);
}

void LockerService::tryUnequipWeapon(const Uuid &weapon) const
{
  const auto data = m_repositories.shipWeaponRepository->findOneByShipAndWeapon(*m_playerShipDbId,
                                                                                weapon);
  log("Uninstalling weapon " + str(data->weapon) + " from slot " + str(data->slot));
  m_repositories.shipWeaponRepository->deleteByShipAndSlot(*data);
}

void LockerService::tryUnequipComputer(const Uuid &computer) const
{
  log("Uninstalling computer " + str(computer) + " from ship " + str(*m_playerShipDbId));
  m_repositories.shipComputerRepository->deleteByShipAndId(*m_playerShipDbId, computer);
}

} // namespace bsgo
