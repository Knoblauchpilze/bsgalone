
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "PurchaseMessage.hh"
#include "Services.hh"

namespace bsgalone::server {

class PurchaseMessageConsumer : public core::AbstractMessageConsumer
{
  public:
  PurchaseMessageConsumer(const Services &services,
                          core::IMessageQueue *const systemMessageQueue,
                          core::IMessageQueue *const outputMessageQueue);
  ~PurchaseMessageConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  PurchaseServiceShPtr m_purchaseService{};
  core::IMessageQueue *const m_systemMessageQueue{};
  core::IMessageQueue *const m_outputMessageQueue{};

  void handleComputerPurchase(const core::PurchaseMessage &message) const;
  void handleShipPurchase(const core::PurchaseMessage &message) const;
  void handleWeaponPurchase(const core::PurchaseMessage &message) const;

  void handleSuccessfulPurchase(const core::PurchaseMessage &message) const;
};

} // namespace bsgalone::server
