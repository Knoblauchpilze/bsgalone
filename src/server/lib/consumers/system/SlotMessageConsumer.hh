
#pragma once

#include "AbstractMessageConsumer.hh"
#include "Services.hh"
#include "SlotMessage.hh"

namespace bsgo {

class SlotMessageConsumer : public bsgalone::core::AbstractMessageConsumer
{
  public:
  SlotMessageConsumer(const Services &services,
                      bsgalone::core::IMessageQueue *const outputMessageQueue);
  ~SlotMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  SlotServiceShPtr m_slotService{};
  bsgalone::core::IMessageQueue *const m_outputMessageQueue{};

  void handleWeapon(const SlotMessage &message) const;
  void handleComputer(const SlotMessage &message) const;
};

} // namespace bsgo
