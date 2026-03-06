
#pragma once

#include "AbstractMessageConsumer.hh"
#include "EquipMessage.hh"
#include "IMessageQueue.hh"
#include "Item.hh"
#include "Services.hh"

namespace bsgalone::server {

class EquipMessageConsumer : public core::AbstractMessageConsumer
{
  public:
  EquipMessageConsumer(const Services &services,
                       core::IMessageQueue *const systemMessageQueue,
                       core::IMessageQueue *const outputMessageQueue);
  ~EquipMessageConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  LockerServiceShPtr m_lockerService{};
  ShipServiceShPtr m_shipService{};
  core::IMessageQueue *const m_systemMessageQueue{};
  core::IMessageQueue *const m_outputMessageQueue{};

  void handleEquipRequest(const core::EquipMessage &message) const;
  void handleUnequipRequest(const core::EquipMessage &message) const;

  void handleSuccessfulRequest(const core::EquipMessage &message) const;
};

} // namespace bsgalone::server
