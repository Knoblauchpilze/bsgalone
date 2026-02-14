
#pragma once

#include "AbstractMessageConsumer.hh"
#include "Services.hh"
#include "SlotMessage.hh"

namespace bsgo {

class SlotMessageConsumer : public AbstractMessageConsumer
{
  public:
  SlotMessageConsumer(const Services &services, IMessageQueue *const outputMessageQueue);
  ~SlotMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  SlotServiceShPtr m_slotService{};
  IMessageQueue *const m_outputMessageQueue{};

  void handleWeapon(const SlotMessage &message) const;
  void handleComputer(const SlotMessage &message) const;
};

} // namespace bsgo
