
#include "SlotMessageConsumer.hh"

namespace bsgo {

SlotMessageConsumer::SlotMessageConsumer(const Services &services, IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("slot", {MessageType::SLOT})
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

void SlotMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &slotMessage = message.as<SlotMessage>();

  if (Slot::WEAPON == slotMessage.getSlotType())
  {
    handleWeapon(slotMessage);
  }
  if (Slot::COMPUTER == slotMessage.getSlotType())
  {
    handleComputer(slotMessage);
  }
}

void SlotMessageConsumer::handleWeapon(const SlotMessage &message) const
{
  const auto shipEntityId = message.getShipEntityId();
  const auto weaponId     = message.getSlotIndex();

  if (!m_slotService->tryToggleWeapon(shipEntityId, weaponId))
  {
    warn("Failed to process weapon slot message for ship " + str(shipEntityId));
    return;
  }
}

void SlotMessageConsumer::handleComputer(const SlotMessage &message) const
{
  const auto shipEntityId = message.getShipEntityId();
  const auto computerId   = message.getSlotIndex();

  if (!m_slotService->tryToggleComputer(shipEntityId, computerId))
  {
    warn("Failed to process computer slot message for ship " + str(shipEntityId));
    return;
  }
}

} // namespace bsgo
