
#include "SlotService.hh"

namespace bsgo {

SlotService::SlotService(const Repositories &repositories,
                         CoordinatorShPtr coordinator,
                         const DatabaseEntityMapper &entityMapper)
  : AbstractService("slot", repositories)
  , m_coordinator(std::move(coordinator))
  , m_entityMapper(entityMapper)
{}

bool SlotService::tryToggleWeapon(const Uuid shipDbId, const Uuid weaponDbId) const
{
  const auto maybeEntityId = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeEntityId)
  {
    debug("haha");
    return false;
  }

  const auto ship        = m_coordinator->getEntity(*maybeEntityId);
  const auto maybeWeapon = ship.tryGetWeapon(weaponDbId);
  if (!maybeWeapon)
  {
    debug("hihi");
    return false;
  }

  (*maybeWeapon)->toggle();

  return true;
}

bool SlotService::tryToggleComputer(const Uuid shipDbId, const Uuid computerDbId) const
{
  const auto maybeEntityId = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeEntityId)
  {
    debug("haha");
    return false;
  }

  auto ship = m_coordinator->getEntity(*maybeEntityId);
  debug("ship db id " + str(shipDbId) + ", entity is " + str(*maybeEntityId)
        + ", str: " + ship.str());
  const auto maybeComputer = ship.tryGetComputer(computerDbId);
  if (!maybeComputer)
  {
    debug("hihi");
    return false;
  }

  (*maybeComputer)->registerFireRequest();

  return true;
}

} // namespace bsgo
