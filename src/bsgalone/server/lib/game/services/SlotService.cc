
#include "SlotService.hh"

namespace bsgalone::server {

SlotService::SlotService(const core::Repositories &repositories,
                         core::CoordinatorShPtr coordinator,
                         const core::DatabaseEntityMapper &entityMapper)
  : AbstractService("slot", repositories)
  , m_coordinator(std::move(coordinator))
  , m_entityMapper(entityMapper)
{}

auto SlotService::tryToggleWeapon(const core::Uuid shipDbId, const core::Uuid weaponDbId) const
  -> TogglingResult
{
  const auto maybeEntityId = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeEntityId)
  {
    warn("Failed to toggle weapon " + core::str(weaponDbId) + " for ship " + core::str(shipDbId),
         "Ship is not attached to any entity");
    return {};
  }

  const auto ship        = m_coordinator->getEntity(*maybeEntityId);
  const auto maybeWeapon = ship.tryGetWeapon(weaponDbId);
  if (!maybeWeapon)
  {
    warn("Failed to toggle weapon " + core::str(weaponDbId) + " for ship " + core::str(shipDbId),
         "No such weapon");
    return {};
  }

  (*maybeWeapon)->toggle();

  return TogglingResult{.success = true, .active = (*maybeWeapon)->active()};
}

bool SlotService::tryToggleComputer(const core::Uuid shipDbId, const core::Uuid computerDbId) const
{
  const auto maybeEntityId = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeEntityId)
  {
    warn("Failed to toggle computer " + core::str(computerDbId) + " for ship " + core::str(shipDbId),
         "Ship is not attached to any entity");
    return false;
  }

  auto ship                = m_coordinator->getEntity(*maybeEntityId);
  const auto maybeComputer = ship.tryGetComputer(computerDbId);
  if (!maybeComputer)
  {
    warn("Failed to toggle computer " + core::str(computerDbId) + " for ship " + core::str(shipDbId),
         "No such computer");
    return false;
  }

  (*maybeComputer)->registerFireRequest();

  return true;
}

} // namespace bsgalone::server
