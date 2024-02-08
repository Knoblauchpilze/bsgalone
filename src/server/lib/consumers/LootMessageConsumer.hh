
#pragma once

#include "AbstractMessageConsumer.hh"
#include "CombatService.hh"
#include "IMessageQueue.hh"
#include "LootMessage.hh"

namespace bsgo {

class LootMessageConsumer : public AbstractMessageConsumer
{
  public:
  LootMessageConsumer(CombatServiceShPtr combatService, IMessageQueue *const messageQueue);
  ~LootMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  CombatServiceShPtr m_combatService{};
  IMessageQueue *const m_messageQueue{};
};

} // namespace bsgo
