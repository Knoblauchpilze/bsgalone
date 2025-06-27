
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "PurchaseMessage.hh"
#include "Services.hh"

namespace bsgo {

class PurchaseMessageConsumer : public AbstractMessageConsumer
{
  public:
  PurchaseMessageConsumer(const Services &services, IMessageQueue *const outputMessageQueue);
  ~PurchaseMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  PurchaseServiceShPtr m_purchaseService{};
  IMessageQueue *const m_outputMessageQueue{};

  void handleComputerPurchase(const PurchaseMessage &message) const;
  void handleShipPurchase(const PurchaseMessage &message) const;
  void handleWeaponPurchase(const PurchaseMessage &message) const;
};

} // namespace bsgo
