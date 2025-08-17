
#pragma once

#include "AbstractMessageConsumer.hh"
#include "EquipMessage.hh"
#include "IMessageQueue.hh"
#include "Item.hh"
#include "Services.hh"

namespace bsgo {

class EquipMessageConsumer : public AbstractMessageConsumer
{
  public:
  EquipMessageConsumer(const Services &services,
                       IMessageQueue *const systemMessageQueue,
                       IMessageQueue *const outputMessageQueue);
  ~EquipMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  LockerServiceShPtr m_lockerService{};
  ShipServiceShPtr m_shipService{};
  IMessageQueue *const m_systemMessageQueue{};
  IMessageQueue *const m_outputMessageQueue{};

  void handleEquipRequest(const EquipMessage &message) const;
  void handleUnequipRequest(const EquipMessage &message) const;

  void handleSuccessfulRequest(const EquipMessage &message) const;
};

} // namespace bsgo
