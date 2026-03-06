
#include "ComponentMessageConsumer.hh"
#include "ComponentUpdatedMessage.hh"

namespace bsgalone::client {

ComponentMessageConsumer::ComponentMessageConsumer(const core::DatabaseEntityMapper &entityMapper,
                                                   core::CoordinatorShPtr coordinator)
  : AbstractMessageConsumer("component",
                            {core::MessageType::SLOT_COMPONENT_UPDATED,
                             core::MessageType::WEAPON_COMPONENT_UPDATED,
                             core::MessageType::AI_BEHAVIOR_SYNC})
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
{}

void ComponentMessageConsumer::onEventReceived(const core::IMessage &message)
{
  if (!m_entityMapper.doesPlayerHaveAnEntity())
  {
    // Most probably the player did not undock yet.
    return;
  }

  switch (message.type())
  {
    case core::MessageType::SLOT_COMPONENT_UPDATED:
      handleComputerSlotUpdated(message.as<core::SlotComponentMessage>());
      break;
    case core::MessageType::WEAPON_COMPONENT_UPDATED:
      handleWeaponUpdated(message.as<core::WeaponComponentMessage>());
      break;
    case core::MessageType::AI_BEHAVIOR_SYNC:
      handleAiBehaviorUpdated(message.as<core::AiBehaviorSyncMessage>());
      break;
    default:
      error("Unsupported message type " + str(message.type()));
  }
}

void ComponentMessageConsumer::handleComputerSlotUpdated(
  const core::SlotComponentMessage &message) const
{
  const auto shipDbId     = message.getShipDbId();
  const auto computerDbId = message.getSlotDbId();

  const auto maybeShip = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeShip)
  {
    warn("Failed to process computer component updated message for ship " + core::str(shipDbId));
    return;
  }

  const auto ship          = m_coordinator->getEntity(*maybeShip);
  const auto maybeComputer = ship.tryGetComputer(computerDbId);
  if (!maybeComputer)
  {
    warn("Failed to process slot component updated message for ship " + core::str(shipDbId));
    return;
  }

  (*maybeComputer)->overrideElapsedSinceLastFired(message.getElapsedSinceLastFired());
}

void ComponentMessageConsumer::handleWeaponUpdated(const core::WeaponComponentMessage &message) const
{
  const auto shipDbId   = message.getShipDbId();
  const auto weaponDbId = message.getWeaponDbId();

  const auto maybeShip = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeShip)
  {
    warn("Failed to process weapon component updated message for ship " + core::str(shipDbId));
    return;
  }

  const auto ship        = m_coordinator->getEntity(*maybeShip);
  const auto maybeWeapon = ship.tryGetWeapon(weaponDbId);
  if (!maybeWeapon)
  {
    warn("Failed to process computer component updated message for ship " + core::str(shipDbId));
    return;
  }

  (*maybeWeapon)->setActive(message.isActive());
}

void ComponentMessageConsumer::handleAiBehaviorUpdated(
  const core::AiBehaviorSyncMessage &message) const
{
  const auto shipDbId = message.getShipDbId();

  const auto maybeShip = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeShip)
  {
    warn("Failed to process AI behavior sync message for ship " + core::str(shipDbId));
    return;
  }

  auto ship = m_coordinator->getEntity(*maybeShip);
  if (!ship.exists<core::AiComponent>())
  {
    warn("Failed to process AI behavior sync for ship " + core::str(shipDbId),
         "Ship does not have an AI comp");
    return;
  }

  auto &aiComp      = ship.aiComp();
  auto &dataContext = aiComp.dataContext();

  const auto maybeTargetIndex = message.tryGetTargetIndex();
  if (maybeTargetIndex)
  {
    debug("Ship " + core::str(shipDbId) + " now has target index "
          + std::to_string(*maybeTargetIndex));
    dataContext.setKey(core::ContextKey::TARGET_REACHED, *maybeTargetIndex);
  }
  else
  {
    verbose("Ship " + core::str(shipDbId) + " does not have a target anymore");
    dataContext.clear(core::ContextKey::TARGET_REACHED);
  }
}

} // namespace bsgalone::client
