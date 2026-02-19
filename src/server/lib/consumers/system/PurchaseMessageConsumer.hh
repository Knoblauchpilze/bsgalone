
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "PurchaseMessage.hh"
#include "Services.hh"

namespace bsgo {

class PurchaseMessageConsumer : public bsgalone::core::AbstractMessageConsumer
{
  public:
  PurchaseMessageConsumer(const Services &services,
                          bsgalone::core::IMessageQueue *const systemMessageQueue,
                          bsgalone::core::IMessageQueue *const outputMessageQueue);
  ~PurchaseMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  PurchaseServiceShPtr m_purchaseService{};
  bsgalone::core::IMessageQueue *const m_systemMessageQueue{};
  bsgalone::core::IMessageQueue *const m_outputMessageQueue{};

  void handleComputerPurchase(const bsgalone::core::PurchaseMessage &message) const;
  void handleShipPurchase(const bsgalone::core::PurchaseMessage &message) const;
  void handleWeaponPurchase(const bsgalone::core::PurchaseMessage &message) const;

  void handleSuccessfulPurchase(const bsgalone::core::PurchaseMessage &message) const;
};

} // namespace bsgo
