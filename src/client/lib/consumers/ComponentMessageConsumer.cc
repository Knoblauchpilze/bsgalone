
#include "ComponentMessageConsumer.hh"
#include "ComponentUpdatedMessage.hh"

namespace pge {

ComponentMessageConsumer::ComponentMessageConsumer(const bsgo::DatabaseEntityMapper &entityMapper,
                                                   bsgo::CoordinatorShPtr coordinator)
  : bsgo::AbstractMessageConsumer("component",
                                  {bsgo::MessageType::SLOT_COMPONENT_UPDATED,
                                   bsgo::MessageType::WEAPON_COMPONENT_UPDATED})
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
{}

void ComponentMessageConsumer::onMessageReceived(const bsgo::IMessage &message)
{
  switch (message.type())
  {
    case bsgo::MessageType::SLOT_COMPONENT_UPDATED:
      handleComputerSlotUpdated(message.as<bsgo::SlotComponentMessage>());
      break;
    case bsgo::MessageType::WEAPON_COMPONENT_UPDATED:
      handleWeaponUpdated(message.as<bsgo::WeaponComponentMessage>());
      break;
    default:
      error("Unsupported message type " + bsgo::str(message.type()));
  }
}

void ComponentMessageConsumer::handleComputerSlotUpdated(
  const bsgo::SlotComponentMessage &message) const
{
  const auto shipDbId     = message.getShipDbId();
  const auto computerDbId = message.getSlotDbId();

  const auto maybeShip = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeShip)
  {
    warn("Failed to process computer component updated message for ship " + bsgo::str(shipDbId));
    return;
  }

  const auto ship          = m_coordinator->getEntity(*maybeShip);
  const auto maybeComputer = ship.tryGetComputer(computerDbId);
  if (!maybeComputer)
  {
    warn("Failed to process slot component updated message for ship " + bsgo::str(shipDbId));
    return;
  }

  (*maybeComputer)->overrideElapsedSinceLastFired(message.getElapsedSinceLastFired());
}

void ComponentMessageConsumer::handleWeaponUpdated(const bsgo::WeaponComponentMessage &message) const
{
  const auto shipDbId   = message.getShipDbId();
  const auto weaponDbId = message.getWeaponDbId();

  const auto maybeShip = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeShip)
  {
    warn("Failed to process weapon component updated message for ship " + bsgo::str(shipDbId));
    return;
  }

  const auto ship        = m_coordinator->getEntity(*maybeShip);
  const auto maybeWeapon = ship.tryGetWeapon(weaponDbId);
  if (!maybeWeapon)
  {
    warn("Failed to process computer component updated message for ship " + bsgo::str(shipDbId));
    return;
  }

  debug("setting " + std::to_string(message.isActive()));

  (*maybeWeapon)->setActive(message.isActive());
}

} // namespace pge
