
#include "EquipMessageConsumer.hh"
#include "EquipMessage.hh"

namespace bsgo {

EquipMessageConsumer::EquipMessageConsumer(const Services &services,
                                           IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("equip", {MessageType::EQUIP})
  , m_lockerService(services.locker)
  , m_messageQueue(messageQueue)
{
  if (nullptr == m_lockerService)
  {
    throw std::invalid_argument("Expected non null locker service");
  }
  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void EquipMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &equip = message.as<EquipMessage>();
  if (equip.validated())
  {
    return;
  }

  const auto shipDbId = equip.getShipDbId();
  const auto itemType = equip.getItemType();
  const auto itemDbId = equip.getItemDbId();

  const auto action = equip.getAction();
  switch (action)
  {
    case EquipType::EQUIP:
      handleEquipRequest(shipDbId, itemType, itemDbId);
      break;
    case EquipType::UNEQUIP:
      handleUnequipRequest(shipDbId, itemType, itemDbId);
      break;
    default:
      error("Unsupported action type for equip message");
  }
}

void EquipMessageConsumer::handleEquipRequest(const Uuid &shipDbId,
                                              const Item &type,
                                              const Uuid &itemDbId) const
{
  const bsgo::LockerItemData data{.dbId = itemDbId, .type = type, .shipDbId = shipDbId};
  if (!m_lockerService->tryEquip(data))
  {
    warn("Failed to process equip message for ship " + str(shipDbId) + " for " + str(type) + " "
         + str(itemDbId));
    return;
  }

  auto message = std::make_unique<EquipMessage>(EquipType::EQUIP, shipDbId, type, itemDbId);
  message->validate();
  m_messageQueue->pushMessage(std::move(message));
}

void EquipMessageConsumer::handleUnequipRequest(const Uuid &shipDbId,
                                                const Item &type,
                                                const Uuid &itemDbId) const
{
  const bsgo::LockerItemData data{.dbId = itemDbId, .type = type, .shipDbId = shipDbId};
  if (!m_lockerService->tryUnequip(data))
  {
    warn("Failed to process unequip message for ship " + str(shipDbId) + " for " + str(type) + " "
         + str(itemDbId));
    return;
  }

  auto message = std::make_unique<EquipMessage>(EquipType::UNEQUIP, shipDbId, type, itemDbId);
  message->validate();
  m_messageQueue->pushMessage(std::move(message));
}

} // namespace bsgo
