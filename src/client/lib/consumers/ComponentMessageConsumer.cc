
#include "ComponentMessageConsumer.hh"
#include "ComponentUpdatedMessage.hh"

namespace pge {

ComponentMessageConsumer::ComponentMessageConsumer(const bsgo::DatabaseEntityMapper &entityMapper,
                                                   bsgo::CoordinatorShPtr coordinator)
  : bsgo::AbstractMessageConsumer("component",
                                  {bsgo::MessageType::SLOT_COMPONENT_UPDATED,
                                   bsgo::MessageType::WEAPON_COMPONENT_UPDATED,
                                   bsgo::MessageType::AI_BEHAVIOR_SYNC})
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
{}

void ComponentMessageConsumer::onMessageReceived(const bsgalone::core::IMessage &message)
{
  if (!m_entityMapper.doesPlayerHaveAnEntity())
  {
    // Most probably the player did not undock yet.
    return;
  }

  switch (message.type())
  {
    case bsgo::MessageType::SLOT_COMPONENT_UPDATED:
      handleComputerSlotUpdated(message.as<bsgo::SlotComponentMessage>());
      break;
    case bsgo::MessageType::WEAPON_COMPONENT_UPDATED:
      handleWeaponUpdated(message.as<bsgo::WeaponComponentMessage>());
      break;
    case bsgo::MessageType::AI_BEHAVIOR_SYNC:
      handleAiBehaviorUpdated(message.as<bsgo::AiBehaviorSyncMessage>());
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

  (*maybeWeapon)->setActive(message.isActive());
}

void ComponentMessageConsumer::handleAiBehaviorUpdated(
  const bsgo::AiBehaviorSyncMessage &message) const
{
  const auto shipDbId = message.getShipDbId();

  const auto maybeShip = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeShip)
  {
    warn("Failed to process AI behavior sync message for ship " + bsgo::str(shipDbId));
    return;
  }

  auto ship = m_coordinator->getEntity(*maybeShip);
  if (!ship.exists<bsgo::AiComponent>())
  {
    warn("Failed to process AI behavior sync for ship " + bsgo::str(shipDbId),
         "Ship does not have an AI comp");
    return;
  }

  auto &aiComp      = ship.aiComp();
  auto &dataContext = aiComp.dataContext();

  const auto maybeTargetIndex = message.tryGetTargetIndex();
  if (maybeTargetIndex)
  {
    debug("Ship " + bsgo::str(shipDbId) + " now has target index "
          + std::to_string(*maybeTargetIndex));
    dataContext.setKey(bsgo::ContextKey::TARGET_REACHED, *maybeTargetIndex);
  }
  else
  {
    verbose("Ship " + bsgo::str(shipDbId) + " does not have a target anymore");
    dataContext.clear(bsgo::ContextKey::TARGET_REACHED);
  }
}

} // namespace pge
