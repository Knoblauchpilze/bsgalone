
#pragma once

#include "AbstractMessageConsumer.hh"
#include "EquipMessage.hh"
#include "IMessageQueue.hh"
#include "Item.hh"
#include "Services.hh"

namespace bsgo {

class EquipMessageConsumer : public bsgalone::core::AbstractMessageConsumer
{
  public:
  EquipMessageConsumer(const Services &services,
                       bsgalone::core::IMessageQueue *const systemMessageQueue,
                       bsgalone::core::IMessageQueue *const outputMessageQueue);
  ~EquipMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  LockerServiceShPtr m_lockerService{};
  ShipServiceShPtr m_shipService{};
  bsgalone::core::IMessageQueue *const m_systemMessageQueue{};
  bsgalone::core::IMessageQueue *const m_outputMessageQueue{};

  void handleEquipRequest(const EquipMessage &message) const;
  void handleUnequipRequest(const EquipMessage &message) const;

  void handleSuccessfulRequest(const EquipMessage &message) const;
};

} // namespace bsgo
