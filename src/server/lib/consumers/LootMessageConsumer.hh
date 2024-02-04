
#pragma once

#include "AbstractMessageConsumer.hh"
#include "CombatService.hh"
#include "IMessageQueue.hh"
#include "LootMessage.hh"

namespace bsgo {

class LootMessageConsumer : public AbstractMessageConsumer
{
  public:
  LootMessageConsumer(CombatServicePtr combatService, IMessageQueue *const messageQueue);
  ~LootMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  CombatServicePtr m_combatService{};
  IMessageQueue *const m_messageQueue{};
};

} // namespace bsgo
