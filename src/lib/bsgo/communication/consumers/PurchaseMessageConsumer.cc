
#include "PurchaseMessageConsumer.hh"
#include "PurchaseMessage.hh"

namespace bsgo {

PurchaseMessageConsumer::PurchaseMessageConsumer(const Services &services,
                                                 IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("purchase", {MessageType::PURCHASE})
  , m_purchaseService(services.purchase)
  , m_messageQueue(messageQueue)
{
  if (nullptr == m_purchaseService)
  {
    throw std::invalid_argument("Expected non null slot service");
  }
  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void PurchaseMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &purchase  = message.as<PurchaseMessage>();
  const auto buyRequest = PurchaseState::REQUESTED == purchase.getPurchaseState();
  if (!buyRequest)
  {
    return;
  }

  const auto playerDbId = purchase.getPlayerDbId();
  const auto itemDbId   = purchase.getItemDbId();

  const auto type = purchase.getItemType();
  switch (type)
  {
    case Item::COMPUTER:
      handleComputerPurchase(playerDbId, itemDbId);
      break;
    case Item::SHIP:
      handleShipPurchase(playerDbId, itemDbId);
      break;
    case Item::WEAPON:
      handleWeaponPurchase(playerDbId, itemDbId);
      break;
    default:
      error("Unsupported item type " + str(type) + " to purchase");
      break;
  }
}

void PurchaseMessageConsumer::handleComputerPurchase(const Uuid &playerDbId,
                                                     const Uuid &computerDbId) const
{
  if (!m_purchaseService->tryPurchase(playerDbId, computerDbId, Item::COMPUTER))
  {
    warn("Failed to process purchase message for player " + str(playerDbId) + " for computer "
         + str(computerDbId));
    return;
  }

  m_messageQueue->pushMessage(std::make_unique<PurchaseMessage>(playerDbId,
                                                                Item::COMPUTER,
                                                                computerDbId,
                                                                PurchaseState::COMPLETED));
}

void PurchaseMessageConsumer::handleShipPurchase(const Uuid &playerDbId, const Uuid &shipDbId) const
{
  if (!m_purchaseService->tryPurchase(playerDbId, shipDbId, Item::SHIP))
  {
    warn("Failed to process purchase message for player " + str(playerDbId) + " for ship "
         + str(shipDbId));
    return;
  }

  m_messageQueue->pushMessage(
    std::make_unique<PurchaseMessage>(playerDbId, Item::SHIP, shipDbId, PurchaseState::COMPLETED));
}

void PurchaseMessageConsumer::handleWeaponPurchase(const Uuid &playerDbId,
                                                   const Uuid &weaponDbId) const
{
  if (!m_purchaseService->tryPurchase(playerDbId, weaponDbId, Item::WEAPON))
  {
    warn("Failed to process purchase message for player " + str(playerDbId) + " for weapon "
         + str(weaponDbId));
    return;
  }

  m_messageQueue->pushMessage(std::make_unique<PurchaseMessage>(playerDbId,
                                                                Item::WEAPON,
                                                                weaponDbId,
                                                                PurchaseState::COMPLETED));
}

} // namespace bsgo
