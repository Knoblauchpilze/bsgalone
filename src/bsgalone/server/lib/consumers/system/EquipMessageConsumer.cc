
#include "EquipMessageConsumer.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"

namespace bsgalone::server {

EquipMessageConsumer::EquipMessageConsumer(const Services &services,
                                           core::IMessageQueue *const systemMessageQueue,
                                           core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("equip", {core::MessageType::EQUIP})
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

void EquipMessageConsumer::onEventReceived(const core::IMessage &message)
{
  const auto &equip = message.as<core::EquipMessage>();
  const auto action = equip.getAction();

  switch (action)
  {
    case core::EquipType::EQUIP:
      handleEquipRequest(equip);
      break;
    case core::EquipType::UNEQUIP:
      handleUnequipRequest(equip);
      break;
    default:
      error("Unsupported action type for equip message");
  }
}

void EquipMessageConsumer::handleEquipRequest(const core::EquipMessage &message) const
{
  const auto shipDbId = message.getShipDbId();
  const auto type     = message.getItemType();
  const auto itemDbId = message.getItemDbId();

  const LockerItemData data{.dbId = itemDbId, .type = type, .shipDbId = shipDbId};
  if (!m_lockerService->tryEquip(data))
  {
    warn("Failed to process equip message for ship " + core::str(shipDbId) + " for " + str(type)
         + " " + core::str(itemDbId));
    return;
  }

  m_outputMessageQueue->pushEvent(message.clone());

  handleSuccessfulRequest(message);
}

void EquipMessageConsumer::handleUnequipRequest(const core::EquipMessage &message) const
{
  const auto shipDbId = message.getShipDbId();
  const auto type     = message.getItemType();
  const auto itemDbId = message.getItemDbId();

  const LockerItemData data{.dbId = itemDbId, .type = type, .shipDbId = shipDbId};
  if (!m_lockerService->tryUnequip(data))
  {
    warn("Failed to process unequip message for ship " + core::str(shipDbId) + " for " + str(type)
         + " " + core::str(itemDbId));
    return;
  }

  m_outputMessageQueue->pushEvent(message.clone());

  handleSuccessfulRequest(message);
}

void EquipMessageConsumer::handleSuccessfulRequest(const core::EquipMessage &message) const
{
  const auto shipDbId   = message.getShipDbId();
  const auto playerDbId = m_shipService->getPlayerDbIdForShip(shipDbId);

  auto started = std::make_unique<core::LoadingStartedMessage>(core::LoadingTransition::EQUIP,
                                                               playerDbId);
  m_systemMessageQueue->pushEvent(std::move(started));

  auto finished = std::make_unique<core::LoadingFinishedMessage>(core::LoadingTransition::EQUIP,
                                                                 playerDbId);
  m_systemMessageQueue->pushEvent(std::move(finished));
}

} // namespace bsgalone::server
