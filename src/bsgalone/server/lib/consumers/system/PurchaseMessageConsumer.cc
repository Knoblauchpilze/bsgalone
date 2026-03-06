
#include "PurchaseMessageConsumer.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"

namespace bsgalone::server {

PurchaseMessageConsumer::PurchaseMessageConsumer(const Services &services,
                                                 core::IMessageQueue *const systemMessageQueue,
                                                 core::IMessageQueue *const outputMessageQueue)
  : AbstractMessageConsumer("purchase", {core::MessageType::PURCHASE})
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

void PurchaseMessageConsumer::onEventReceived(const core::IMessage &message)
{
  const auto &purchase = message.as<core::PurchaseMessage>();
  const auto type      = purchase.getItemType();

  switch (type)
  {
    case core::Item::COMPUTER:
      handleComputerPurchase(purchase);
      break;
    case core::Item::SHIP:
      handleShipPurchase(purchase);
      break;
    case core::Item::WEAPON:
      handleWeaponPurchase(purchase);
      break;
    default:
      error("Unsupported item type " + core::str(type) + " to purchase");
      break;
  }
}

void PurchaseMessageConsumer::handleComputerPurchase(const core::PurchaseMessage &message) const
{
  const auto playerDbId   = message.getPlayerDbId();
  const auto computerDbId = message.getItemDbId();

  if (!m_purchaseService->tryPurchase(playerDbId, computerDbId, core::Item::COMPUTER))
  {
    warn("Failed to process purchase message for player " + core::str(playerDbId) + " for computer "
         + core::str(computerDbId));
    return;
  }

  m_outputMessageQueue->pushEvent(message.clone());

  handleSuccessfulPurchase(message);
}

void PurchaseMessageConsumer::handleShipPurchase(const core::PurchaseMessage &message) const
{
  const auto playerDbId = message.getPlayerDbId();
  const auto shipDbId   = message.getItemDbId();

  if (!m_purchaseService->tryPurchase(playerDbId, shipDbId, core::Item::SHIP))
  {
    warn("Failed to process purchase message for player " + core::str(playerDbId) + " for ship "
         + core::str(shipDbId));
    return;
  }

  m_outputMessageQueue->pushEvent(message.clone());

  handleSuccessfulPurchase(message);
}

void PurchaseMessageConsumer::handleWeaponPurchase(const core::PurchaseMessage &message) const
{
  const auto playerDbId = message.getPlayerDbId();
  const auto weaponDbId = message.getItemDbId();

  if (!m_purchaseService->tryPurchase(playerDbId, weaponDbId, core::Item::WEAPON))
  {
    warn("Failed to process purchase message for player " + core::str(playerDbId) + " for weapon "
         + core::str(weaponDbId));
    return;
  }

  m_outputMessageQueue->pushEvent(message.clone());

  handleSuccessfulPurchase(message);
}

void PurchaseMessageConsumer::handleSuccessfulPurchase(const core::PurchaseMessage &message) const
{
  const auto playerDbId = message.getPlayerDbId();

  auto started = std::make_unique<core::LoadingStartedMessage>(core::LoadingTransition::PURCHASE,
                                                               playerDbId);
  m_systemMessageQueue->pushEvent(std::move(started));

  auto finished = std::make_unique<core::LoadingFinishedMessage>(core::LoadingTransition::PURCHASE,
                                                                 playerDbId);
  m_systemMessageQueue->pushEvent(std::move(finished));
}

} // namespace bsgalone::server
