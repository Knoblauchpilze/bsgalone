
#include "PurchaseMessageConsumer.hh"

namespace bsgo {

PurchaseMessageConsumer::PurchaseMessageConsumer(const Services &services,
                                                 IMessageQueue *const messageQueue)
  : AbstractMessageConsumer("purchase", {MessageType::PURCHASE})
  , m_purchaseService(services.purchase)
  , m_messageQueue(messageQueue)
{
  if (nullptr == m_purchaseService)
  {
    throw std::invalid_argument("Expected non null purchase service");
  }
  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void PurchaseMessageConsumer::onMessageReceived(const IMessage &message)
{
  const auto &purchase = message.as<PurchaseMessage>();
  if (purchase.validated())
  {
    return;
  }

  const auto type = purchase.getItemType();
  switch (type)
  {
    case Item::COMPUTER:
      handleComputerPurchase(purchase);
      break;
    case Item::SHIP:
      handleShipPurchase(purchase);
      break;
    case Item::WEAPON:
      handleWeaponPurchase(purchase);
      break;
    default:
      error("Unsupported item type " + str(type) + " to purchase");
      break;
  }
}

void PurchaseMessageConsumer::handleComputerPurchase(const PurchaseMessage &message) const
{
  const auto playerDbId   = message.getPlayerDbId();
  const auto computerDbId = message.getItemDbId();

  if (!m_purchaseService->tryPurchase(playerDbId, computerDbId, Item::COMPUTER))
  {
    warn("Failed to process purchase message for player " + str(playerDbId) + " for computer "
         + str(computerDbId));
    return;
  }

  auto out = std::make_unique<PurchaseMessage>(playerDbId, Item::COMPUTER, computerDbId);
  out->validate();
  out->copyClientIdIfDefined(message);
  m_messageQueue->pushMessage(std::move(out));
}

void PurchaseMessageConsumer::handleShipPurchase(const PurchaseMessage &message) const
{
  const auto playerDbId = message.getPlayerDbId();
  const auto shipDbId   = message.getItemDbId();

  if (!m_purchaseService->tryPurchase(playerDbId, shipDbId, Item::SHIP))
  {
    warn("Failed to process purchase message for player " + str(playerDbId) + " for ship "
         + str(shipDbId));
    return;
  }

  auto out = std::make_unique<PurchaseMessage>(playerDbId, Item::SHIP, shipDbId);
  out->validate();
  out->copyClientIdIfDefined(message);
  m_messageQueue->pushMessage(std::move(out));
}

void PurchaseMessageConsumer::handleWeaponPurchase(const PurchaseMessage &message) const
{
  const auto playerDbId = message.getPlayerDbId();
  const auto weaponDbId = message.getItemDbId();

  if (!m_purchaseService->tryPurchase(playerDbId, weaponDbId, Item::WEAPON))
  {
    warn("Failed to process purchase message for player " + str(playerDbId) + " for weapon "
         + str(weaponDbId));
    return;
  }

  auto out = std::make_unique<PurchaseMessage>(playerDbId, Item::WEAPON, weaponDbId);
  out->validate();
  out->copyClientIdIfDefined(message);
  m_messageQueue->pushMessage(std::move(out));
}

} // namespace bsgo
