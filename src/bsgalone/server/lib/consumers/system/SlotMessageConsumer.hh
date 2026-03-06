
#pragma once

#include "AbstractMessageConsumer.hh"
#include "Services.hh"
#include "SlotMessage.hh"

namespace bsgalone::server {

class SlotMessageConsumer : public core::AbstractMessageConsumer
{
  public:
  SlotMessageConsumer(const Services &services, core::IMessageQueue *const outputMessageQueue);
  ~SlotMessageConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  SlotServiceShPtr m_slotService{};
  core::IMessageQueue *const m_outputMessageQueue{};

  void handleWeapon(const core::SlotMessage &message) const;
  void handleComputer(const core::SlotMessage &message) const;
};

} // namespace bsgalone::server
