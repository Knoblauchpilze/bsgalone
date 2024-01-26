
#include "EquipMessageConsumer.hh"

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

  const auto action = equip.getAction();
  switch (action)
  {
    case EquipType::EQUIP:
      handleEquipRequest(equip);
      break;
    case EquipType::UNEQUIP:
      handleUnequipRequest(equip);
      break;
    default:
      error("Unsupported action type for equip message");
  }
}

void EquipMessageConsumer::handleEquipRequest(const EquipMessage &message) const
{
  const auto shipDbId = message.getShipDbId();
  const auto type     = message.getItemType();
  const auto itemDbId = message.getItemDbId();

  const bsgo::LockerItemData data{.dbId = itemDbId, .type = type, .shipDbId = shipDbId};
  if (!m_lockerService->tryEquip(data))
  {
    warn("Failed to process equip message for ship " + str(shipDbId) + " for " + str(type) + " "
         + str(itemDbId));
    return;
  }

  auto out = std::make_unique<EquipMessage>(EquipType::EQUIP, shipDbId, type, itemDbId);
  out->validate();
  out->copyClientIdIfDefined(message);
  m_messageQueue->pushMessage(std::move(out));
}

void EquipMessageConsumer::handleUnequipRequest(const EquipMessage &message) const
{
  const auto shipDbId = message.getShipDbId();
  const auto type     = message.getItemType();
  const auto itemDbId = message.getItemDbId();

  const bsgo::LockerItemData data{.dbId = itemDbId, .type = type, .shipDbId = shipDbId};
  if (!m_lockerService->tryUnequip(data))
  {
    warn("Failed to process unequip message for ship " + str(shipDbId) + " for " + str(type) + " "
         + str(itemDbId));
    return;
  }

  auto out = std::make_unique<EquipMessage>(EquipType::UNEQUIP, shipDbId, type, itemDbId);
  out->validate();
  out->copyClientIdIfDefined(message);
  m_messageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
