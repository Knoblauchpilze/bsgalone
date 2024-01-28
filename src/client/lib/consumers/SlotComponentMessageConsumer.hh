
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "Services.hh"
#include "SlotComponentMessage.hh"

namespace pge {

class SlotComponentMessageConsumer : public bsgo::AbstractMessageConsumer
{
  public:
  SlotComponentMessageConsumer(const bsgo::Services &services,
                               bsgo::IMessageQueue *const messageQueue);
  ~SlotComponentMessageConsumer() override = default;

  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  bsgo::SlotServiceShPtr m_slotService{};
  bsgo::IMessageQueue *const m_messageQueue{};

  void handleComputerSlotUpdated(const bsgo::SlotComponentMessage &message) const;
};

} // namespace pge
