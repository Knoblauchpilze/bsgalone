
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "Services.hh"

namespace bsgo {

class PurchaseMessageConsumer : public AbstractMessageConsumer
{
  public:
  PurchaseMessageConsumer(const Services &services, IMessageQueue *const messageQueue);
  ~PurchaseMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  PurchaseServiceShPtr m_purchaseService{};
  IMessageQueue *const m_messageQueue{};

  void handleComputerPurchase(const Uuid &playerDbId, const Uuid &computerDbId) const;
  void handleWeaponPurchase(const Uuid &playerDbId, const Uuid &weaponDbId) const;
};

} // namespace bsgo
