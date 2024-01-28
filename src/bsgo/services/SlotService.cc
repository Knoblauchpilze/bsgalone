
#include "SlotService.hh"

namespace bsgo {

SlotService::SlotService(const Repositories &repositories, const CoordinatorShPtr &coordinator)
  : AbstractService("slot", repositories, coordinator)
{}

bool SlotService::tryToggleWeapon(const Uuid shipEntityId, const int weaponIndex) const
{
  const auto ship = m_coordinator->getEntity(shipEntityId);
  if (ship.weapons.size() < static_cast<std::size_t>(weaponIndex))
  {
    return false;
  }

  ship.weapons[weaponIndex]->toggle();

  return true;
}

bool SlotService::tryToggleComputer(const Uuid shipEntityId, const int computerIndex) const
{
  auto ship = m_coordinator->getEntity(shipEntityId);
  if (ship.computers.size() < static_cast<std::size_t>(computerIndex))
  {
    return false;
  }

  ship.computers[computerIndex]->registerFireRequest();

  return true;
}

bool SlotService::trySyncComputer(const Uuid shipEntityId,
                                  const int computerIndex,
                                  const SlotUpdateData &data) const
{
  auto ship = m_coordinator->getEntity(shipEntityId);
  if (ship.computers.size() < static_cast<std::size_t>(computerIndex))
  {
    return false;
  }

  const auto &component = ship.computers[computerIndex];
  component->overrideElapsedSinceLastFired(data.elapsedSinceLastFired);
  debug("override");

  return true;
}

} // namespace bsgo
