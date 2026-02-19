
#include "EquipMessageConsumer.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"

namespace bsgo {

EquipMessageConsumer::EquipMessageConsumer(const Services &services,
                                           bsgalone::core::IMessageQueue *const systemMessageQueue,
                                           bsgalone::core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("equip", {bsgalone::core::MessageType::EQUIP})
  , m_lockerService(services.locker)
  , m_shipService(services.ship)
  , m_systemMessageQueue(systemMessageQueue)
  , m_outputMessageQueue(outputMessageQueue)
{
  if (nullptr == m_lockerService)
  {
    throw std::invalid_argument("Expected non null locker service");
  }
  if (nullptr == m_shipService)
  {
    throw std::invalid_argument("Expected non null ship service");
  }
  if (nullptr == m_systemMessageQueue)
  {
    throw std::invalid_argument("Expected non null system message queue");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void EquipMessageConsumer::onMessageReceived(const bsgalone::core::IMessage &message)
{
  const auto &equip = message.as<bsgalone::core::EquipMessage>();
  const auto action = equip.getAction();

  switch (action)
  {
    case bsgalone::core::EquipType::EQUIP:
      handleEquipRequest(equip);
      break;
    case bsgalone::core::EquipType::UNEQUIP:
      handleUnequipRequest(equip);
      break;
    default:
      error("Unsupported action type for equip message");
  }
}

void EquipMessageConsumer::handleEquipRequest(const bsgalone::core::EquipMessage &message) const
{
  const auto shipDbId = message.getShipDbId();
  const auto type     = message.getItemType();
  const auto itemDbId = message.getItemDbId();

  const LockerItemData data{.dbId = itemDbId, .type = type, .shipDbId = shipDbId};
  if (!m_lockerService->tryEquip(data))
  {
    warn("Failed to process equip message for ship " + str(shipDbId) + " for " + str(type) + " "
         + str(itemDbId));
    return;
  }

  m_outputMessageQueue->pushMessage(message.clone());

  handleSuccessfulRequest(message);
}

void EquipMessageConsumer::handleUnequipRequest(const bsgalone::core::EquipMessage &message) const
{
  const auto shipDbId = message.getShipDbId();
  const auto type     = message.getItemType();
  const auto itemDbId = message.getItemDbId();

  const LockerItemData data{.dbId = itemDbId, .type = type, .shipDbId = shipDbId};
  if (!m_lockerService->tryUnequip(data))
  {
    warn("Failed to process unequip message for ship " + str(shipDbId) + " for " + str(type) + " "
         + str(itemDbId));
    return;
  }

  m_outputMessageQueue->pushMessage(message.clone());

  handleSuccessfulRequest(message);
}

void EquipMessageConsumer::handleSuccessfulRequest(const bsgalone::core::EquipMessage &message) const
{
  const auto shipDbId   = message.getShipDbId();
  const auto playerDbId = m_shipService->getPlayerDbIdForShip(shipDbId);

  auto started = std::make_unique<LoadingStartedMessage>(LoadingTransition::EQUIP, playerDbId);
  m_systemMessageQueue->pushMessage(std::move(started));

  auto finished = std::make_unique<LoadingFinishedMessage>(LoadingTransition::EQUIP, playerDbId);
  m_systemMessageQueue->pushMessage(std::move(finished));
}

} // namespace bsgo
