
#include "LockerService.hh"
#include "LockerUtils.hh"

namespace bsgo {

LockerService::LockerService(const Repositories &repositories)
  : AbstractService("locker", repositories)
{}

void LockerService::setPlayerShipDbId(const Uuid &ship)
{
  m_playerShipDbId = ship;
}

bool LockerService::isReady() const noexcept
{
  return m_playerShipDbId.has_value();
}

bool LockerService::tryEquip(const Uuid &id, const Item &type) const
{
  checkPlayerShipDbIdExists();

  if (!verifySlotAvailability(type))
  {
    return false;
  }

  const auto slot = determineNextSlot(type);
  switch (type)
  {
    case Item::WEAPON:
      tryEquipWeapon(id, slot);
      break;
    default:
      error("Invalid kind of item to equip", "Unsupported item " + str(type));
      break;
  }

  return true;
}

bool LockerService::tryUnequip(const Uuid &id, const Item &type) const
{
  checkPlayerShipDbIdExists();

  if (!verifyItemIsEquiped(id, type))
  {
    return false;
  }

  switch (type)
  {
    case Item::WEAPON:
      tryUnequipWeapon(id);
      break;
    default:
      error("Invalid kind of item to unequip", "Unsupported item " + str(type));
      break;
  }

  return true;
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
  EquipData data{.shipId         = *m_playerShipDbId,
                 .shipWeaponRepo = m_repositories.shipWeaponRepository,
                 .playerShipRepo = m_repositories.playerShipRepository};

  switch (type)
  {
    case Item::WEAPON:
      return canStillEquipWeapon(data);
    default:
      error("Failed to verify slot availability", "Unsupported item " + str(type));
  }

  // Redundant because of the error above.
  return false;
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
    default:
      error("Failed to verify slot availability", "Unsupported item " + str(type));
  }

  return equiped;
}

auto LockerService::determineNextSlot(const Item &type) const -> Uuid
{
  std::set<Uuid> slots{};

  switch (type)
  {
    case Item::WEAPON:
      slots = m_repositories.playerShipRepository->findAllAvailableByIdAndType(*m_playerShipDbId,
                                                                               Slot::WEAPON);
      break;
    default:
      error("Failed to determine available slot", "Unsupported item " + str(type));
  }

  if (slots.empty())
  {
    error("Failed to find available slot for " + str(type));
  }

  return *slots.begin();
}

void LockerService::tryEquipWeapon(const Uuid &id, const Uuid slot) const
{
  ShipWeapon weapon{.ship = *m_playerShipDbId, .weapon = id, .slot = slot};
  log("Installing weapon " + str(id) + " in slot " + str(slot));
  m_repositories.shipWeaponRepository->save(weapon);
}

void LockerService::tryUnequipWeapon(const Uuid &weapon) const
{
  const auto data = m_repositories.shipWeaponRepository->findOneByShipAndWeapon(*m_playerShipDbId,
                                                                                weapon);
  log("Uninstalling weapon " + str(data->weapon) + " from slot " + str(data->slot));
  m_repositories.shipWeaponRepository->deleteByShipAndSlot(*data);
}

} // namespace bsgo
