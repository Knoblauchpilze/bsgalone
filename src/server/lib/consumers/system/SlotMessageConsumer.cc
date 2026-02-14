
#include "SlotMessageConsumer.hh"
#include "WeaponComponentMessage.hh"

namespace bsgo {

SlotMessageConsumer::SlotMessageConsumer(const Services &services,
                                         IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("slot", {MessageType::SLOT})
  , m_slotService(services.slot)
  , m_outputMessageQueue(outputMessageQueue)
{
  if (nullptr == m_slotService)
  {
    throw std::invalid_argument("Expected non null slot service");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void SlotMessageConsumer::onMessageReceived(const bsgalone::core::IMessage &message)
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
  const auto shipDbId   = message.getShipDbId();
  const auto weaponDbId = message.getSlotDbId();

  const auto res = m_slotService->tryToggleWeapon(shipDbId, weaponDbId);
  if (!res.success)
  {
    warn("Failed to process weapon slot message for ship " + str(shipDbId));
    return;
  }

  auto out = std::make_unique<WeaponComponentMessage>(shipDbId, weaponDbId, res.active);
  out->copyClientIdIfDefined(message);
  m_outputMessageQueue->pushMessage(std::move(out));
}

void SlotMessageConsumer::handleComputer(const SlotMessage &message) const
{
  const auto shipDbId     = message.getShipDbId();
  const auto computerDbId = message.getSlotDbId();

  if (!m_slotService->tryToggleComputer(shipDbId, computerDbId))
  {
    warn("Failed to process computer slot message for ship " + str(shipDbId));
    return;
  }
}

} // namespace bsgo
