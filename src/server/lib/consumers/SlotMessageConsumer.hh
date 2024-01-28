
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "Services.hh"
#include "SlotMessage.hh"

namespace bsgo {

class SlotMessageConsumer : public AbstractMessageConsumer
{
  public:
  SlotMessageConsumer(const Services &services, IMessageQueue *const messageQueue);
  ~SlotMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  SlotServiceShPtr m_slotService{};
  IMessageQueue *const m_messageQueue{};

  void handleWeapon(const SlotMessage &message) const;
  void handleComputer(const SlotMessage &message) const;
};

} // namespace bsgo
