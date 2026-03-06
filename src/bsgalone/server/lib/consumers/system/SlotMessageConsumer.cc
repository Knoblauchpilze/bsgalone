
#include "SlotMessageConsumer.hh"
#include "WeaponComponentMessage.hh"

namespace bsgalone::server {

SlotMessageConsumer::SlotMessageConsumer(const Services &services,
                                         core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("slot", {core::MessageType::SLOT})
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

void SlotMessageConsumer::onEventReceived(const core::IMessage &message)
{
  const auto &slotMessage = message.as<core::SlotMessage>();

  if (core::Slot::WEAPON == slotMessage.getSlotType())
  {
    handleWeapon(slotMessage);
  }
  if (core::Slot::COMPUTER == slotMessage.getSlotType())
  {
    handleComputer(slotMessage);
  }
}

void SlotMessageConsumer::handleWeapon(const core::SlotMessage &message) const
{
  const auto shipDbId   = message.getShipDbId();
  const auto weaponDbId = message.getSlotDbId();

  const auto res = m_slotService->tryToggleWeapon(shipDbId, weaponDbId);
  if (!res.success)
  {
    warn("Failed to process weapon slot message for ship " + core::str(shipDbId));
    return;
  }

  auto out = std::make_unique<core::WeaponComponentMessage>(shipDbId, weaponDbId, res.active);
  m_outputMessageQueue->pushEvent(std::move(out));
}

void SlotMessageConsumer::handleComputer(const core::SlotMessage &message) const
{
  const auto shipDbId     = message.getShipDbId();
  const auto computerDbId = message.getSlotDbId();

  if (!m_slotService->tryToggleComputer(shipDbId, computerDbId))
  {
    warn("Failed to process computer slot message for ship " + core::str(shipDbId));
    return;
  }
}

} // namespace bsgalone::server
