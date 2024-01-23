
#include "SlotMessageConsumer.hh"
#include "SlotMessage.hh"

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

  if (Slot::WEAPON == slotMessage.getSlotType() && !slotMessage.validated())
  {
    handleWeapon(slotMessage.getShipEntityId(), slotMessage.getSlotIndex());
  }
  if (Slot::COMPUTER == slotMessage.getSlotType() && !slotMessage.validated())
  {
    handleComputer(slotMessage.getShipEntityId(), slotMessage.getSlotIndex());
  }
}

void SlotMessageConsumer::handleWeapon(const Uuid &shipEntityId, const int weaponId) const
{
  if (!m_slotService->tryToggleWeapon(shipEntityId, weaponId))
  {
    warn("Failed to process weapon slot message for ship " + str(shipEntityId));
    return;
  }

  auto message = std::make_unique<SlotMessage>(shipEntityId, weaponId, Slot::WEAPON);
  message->validate();
  m_messageQueue->pushMessage(std::move(message));
}

void SlotMessageConsumer::handleComputer(const Uuid &shipEntityId, const int computerId) const
{
  if (!m_slotService->tryToggleComputer(shipEntityId, computerId))
  {
    warn("Failed to process computer slot message for ship " + str(shipEntityId));
    return;
  }

  auto message = std::make_unique<SlotMessage>(shipEntityId, computerId, Slot::COMPUTER);
  message->validate();
  m_messageQueue->pushMessage(std::move(message));
}

} // namespace bsgo
