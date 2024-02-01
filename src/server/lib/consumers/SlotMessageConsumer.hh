
#pragma once

#include "AbstractMessageConsumer.hh"
#include "Services.hh"
#include "SlotMessage.hh"

namespace bsgo {

class SlotMessageConsumer : public AbstractMessageConsumer
{
  public:
  SlotMessageConsumer(const Services &services);
  ~SlotMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  SlotServiceShPtr m_slotService{};

  void handleWeapon(const SlotMessage &message) const;
  void handleComputer(const SlotMessage &message) const;
};

} // namespace bsgo
