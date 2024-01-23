
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "Services.hh"

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

  void handleWeapon(const Uuid &shipEntityId, const int weaponId) const;
  void handleComputer(const Uuid &shipEntityId, const int computerId) const;
};

} // namespace bsgo
