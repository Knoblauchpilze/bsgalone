
#include "PurchaseMessageConsumer.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"

namespace bsgo {

PurchaseMessageConsumer::PurchaseMessageConsumer(
  const Services &services,
  bsgalone::core::IMessageQueue *const systemMessageQueue,
  bsgalone::core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("purchase", {bsgalone::core::MessageType::PURCHASE})
  , m_purchaseService(services.purchase)
  , m_systemMessageQueue(systemMessageQueue)
  , m_outputMessageQueue(outputMessageQueue)
{
  if (nullptr == m_purchaseService)
  {
    throw std::invalid_argument("Expected non null purchase service");
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

void PurchaseMessageConsumer::onMessageReceived(const bsgalone::core::IMessage &message)
{
  const auto &purchase = message.as<PurchaseMessage>();
  const auto type      = purchase.getItemType();

  switch (type)
  {
    case bsgalone::core::Item::COMPUTER:
      handleComputerPurchase(purchase);
      break;
    case bsgalone::core::Item::SHIP:
      handleShipPurchase(purchase);
      break;
    case bsgalone::core::Item::WEAPON:
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

  if (!m_purchaseService->tryPurchase(playerDbId, computerDbId, bsgalone::core::Item::COMPUTER))
  {
    warn("Failed to process purchase message for player " + str(playerDbId) + " for computer "
         + str(computerDbId));
    return;
  }

  auto out = std::make_unique<PurchaseMessage>(playerDbId,
                                               bsgalone::core::Item::COMPUTER,
                                               computerDbId);
  out->copyClientIdIfDefined(message);
  m_outputMessageQueue->pushMessage(std::move(out));

  handleSuccessfulPurchase(message);
}

void PurchaseMessageConsumer::handleShipPurchase(const PurchaseMessage &message) const
{
  const auto playerDbId = message.getPlayerDbId();
  const auto shipDbId   = message.getItemDbId();

  if (!m_purchaseService->tryPurchase(playerDbId, shipDbId, bsgalone::core::Item::SHIP))
  {
    warn("Failed to process purchase message for player " + str(playerDbId) + " for ship "
         + str(shipDbId));
    return;
  }

  auto out = std::make_unique<PurchaseMessage>(playerDbId, bsgalone::core::Item::SHIP, shipDbId);
  out->copyClientIdIfDefined(message);
  m_outputMessageQueue->pushMessage(std::move(out));

  handleSuccessfulPurchase(message);
}

void PurchaseMessageConsumer::handleWeaponPurchase(const PurchaseMessage &message) const
{
  const auto playerDbId = message.getPlayerDbId();
  const auto weaponDbId = message.getItemDbId();

  if (!m_purchaseService->tryPurchase(playerDbId, weaponDbId, bsgalone::core::Item::WEAPON))
  {
    warn("Failed to process purchase message for player " + str(playerDbId) + " for weapon "
         + str(weaponDbId));
    return;
  }

  auto out = std::make_unique<PurchaseMessage>(playerDbId, bsgalone::core::Item::WEAPON, weaponDbId);
  out->copyClientIdIfDefined(message);
  m_outputMessageQueue->pushMessage(std::move(out));

  handleSuccessfulPurchase(message);
}

void PurchaseMessageConsumer::handleSuccessfulPurchase(const PurchaseMessage &message) const
{
  const auto playerDbId = message.getPlayerDbId();

  auto started = std::make_unique<LoadingStartedMessage>(LoadingTransition::PURCHASE, playerDbId);
  started->copyClientIdIfDefined(message);
  m_systemMessageQueue->pushMessage(std::move(started));

  auto finished = std::make_unique<LoadingFinishedMessage>(LoadingTransition::PURCHASE, playerDbId);
  finished->copyClientIdIfDefined(message);
  m_systemMessageQueue->pushMessage(std::move(finished));
}

} // namespace bsgo
