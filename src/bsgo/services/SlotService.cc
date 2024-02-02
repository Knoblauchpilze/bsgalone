
#include "SlotService.hh"

namespace bsgo {

SlotService::SlotService(const Repositories &repositories,
                         CoordinatorShPtr coordinator,
                         const DatabaseEntityMapper &entityMapper)
  : AbstractService("slot", repositories)
  , m_coordinator(std::move(coordinator))
  , m_entityMapper(entityMapper)
{}

auto SlotService::tryToggleWeapon(const Uuid shipDbId, const Uuid weaponDbId) const
  -> TogglingResult
{
  const auto maybeEntityId = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeEntityId)
  {
    return {};
  }

  const auto ship        = m_coordinator->getEntity(*maybeEntityId);
  const auto maybeWeapon = ship.tryGetWeapon(weaponDbId);
  if (!maybeWeapon)
  {
    return {};
  }

  (*maybeWeapon)->toggle();

  return TogglingResult{.success = true, .active = (*maybeWeapon)->active()};
}

bool SlotService::tryToggleComputer(const Uuid shipDbId, const Uuid computerDbId) const
{
  const auto maybeEntityId = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeEntityId)
  {
    return false;
  }

  auto ship                = m_coordinator->getEntity(*maybeEntityId);
  const auto maybeComputer = ship.tryGetComputer(computerDbId);
  if (!maybeComputer)
  {
    return false;
  }

  (*maybeComputer)->registerFireRequest();

  return true;
}

} // namespace bsgo
