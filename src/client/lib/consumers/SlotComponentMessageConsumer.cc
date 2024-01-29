
#include "SlotComponentMessageConsumer.hh"

namespace pge {

SlotComponentMessageConsumer::SlotComponentMessageConsumer(const bsgo::Services &services,
                                                           bsgo::IMessageQueue *const messageQueue)
  : bsgo::AbstractMessageConsumer("slot", {bsgo::MessageType::SLOT_COMPONENT_UPDATED})
  , m_slotService(services.slot)
  , m_messageQueue(messageQueue)
{
  if (nullptr == m_slotService)
  {
    throw std::invalid_argument("Expected non null slot service");
  }
  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

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
  /// TODO: Should find the entity associated to the player and handle the update.
  warn("should handle computer updated for " + bsgo::str(message.getPlayerDbId()) + " on ship "
       + bsgo::str(message.getShipDbId()) + " for slot " + bsgo::str(message.getSlotDbId()));
  // const auto entityId   = message.getEntityId();
  // const auto computerId = message.getSlotIndex();

  // const bsgo::SlotService::SlotUpdateData data{
  //   .elapsedSinceLastFired = message.getElapsedSinceLastFired()};

  // if (!m_slotService->trySyncComputer(entityId, computerId, data))
  // {
  //   warn("Failed to process computer component updated message for entity " + bsgo::str(entityId));
  // }
}

} // namespace pge
