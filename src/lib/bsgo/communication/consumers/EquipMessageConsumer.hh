
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "Item.hh"
#include "Services.hh"

namespace bsgo {

class EquipMessageConsumer : public AbstractMessageConsumer
{
  public:
  EquipMessageConsumer(const Services &services, IMessageQueue *const messageQueue);
  ~EquipMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  LockerServiceShPtr m_lockerService{};
  IMessageQueue *const m_messageQueue{};

  void handleEquipRequest(const Uuid &shipDbId, const Item &type, const Uuid &itemDbId) const;
  void handleUnequipRequest(const Uuid &shipDbId, const Item &type, const Uuid &itemDbId) const;
};

} // namespace bsgo
