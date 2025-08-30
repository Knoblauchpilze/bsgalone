
#include "EquipMessageConsumer.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"

namespace bsgo {

EquipMessageConsumer::EquipMessageConsumer(const Services &services,
                                           IMessageQueue *const systemMessageQueue,
                                           IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("equip", {MessageType::EQUIP})
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

  const LockerItemData data{.dbId = itemDbId, .type = type, .shipDbId = shipDbId};
  if (!m_lockerService->tryEquip(data))
  {
    warn("Failed to process equip message for ship " + str(shipDbId) + " for " + str(type) + " "
         + str(itemDbId));
    return;
  }

  auto out = std::make_unique<EquipMessage>(EquipType::EQUIP, shipDbId, type, itemDbId);
  out->validate();
  out->copyClientIdIfDefined(message);
  m_outputMessageQueue->pushMessage(std::move(out));

  handleSuccessfulRequest(message);
}

void EquipMessageConsumer::handleUnequipRequest(const EquipMessage &message) const
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

  auto out = std::make_unique<EquipMessage>(EquipType::UNEQUIP, shipDbId, type, itemDbId);
  out->validate();
  out->copyClientIdIfDefined(message);
  m_outputMessageQueue->pushMessage(std::move(out));

  handleSuccessfulRequest(message);
}

void EquipMessageConsumer::handleSuccessfulRequest(const EquipMessage &message) const
{
  const auto shipDbId        = message.getShipDbId();
  const auto maybePlayerDbId = m_shipService->tryGetPlayerDbIdForShip(shipDbId);
  if (!maybePlayerDbId)
  {
    error("Expected ship " + str(shipDbId) + " to belong to a player");
  }

  auto started = std::make_unique<LoadingStartedMessage>(LoadingTransition::EQUIP);
  started->setPlayerDbId(*maybePlayerDbId);
  started->copyClientIdIfDefined(message);
  m_systemMessageQueue->pushMessage(std::move(started));

  auto finished = std::make_unique<LoadingFinishedMessage>(LoadingTransition::EQUIP);
  finished->setPlayerDbId(*maybePlayerDbId);
  finished->copyClientIdIfDefined(message);
  m_systemMessageQueue->pushMessage(std::move(finished));
}

} // namespace bsgo
