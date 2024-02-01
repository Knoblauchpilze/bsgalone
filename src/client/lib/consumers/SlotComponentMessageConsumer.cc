
#include "SlotComponentMessageConsumer.hh"

namespace pge {

SlotComponentMessageConsumer::SlotComponentMessageConsumer(
  const bsgo::DatabaseEntityMapper &entityMapper,
  bsgo::CoordinatorShPtr coordinator)
  : bsgo::AbstractMessageConsumer("slot", {bsgo::MessageType::SLOT_COMPONENT_UPDATED})
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
{}

void SlotComponentMessageConsumer::onMessageReceived(const bsgo::IMessage &message)
{
  const auto &slot = message.as<bsgo::SlotComponentMessage>();

  switch (slot.getComponentType())
  {
    case bsgo::ComponentType::COMPUTER_SLOT:
      handleComputerSlotUpdated(slot);
      break;
    default:
      error("Unsupported component type " + bsgo::str(slot.getComponentType()));
  }
}

void SlotComponentMessageConsumer::handleComputerSlotUpdated(
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
    warn("Failed to process computer component updated message for ship " + bsgo::str(shipDbId));
    return;
  }

  (*maybeComputer)->overrideElapsedSinceLastFired(message.getElapsedSinceLastFired());
}

} // namespace pge
