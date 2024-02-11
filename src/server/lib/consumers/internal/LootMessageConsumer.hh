
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "LootMessage.hh"
#include "SystemService.hh"

namespace bsgo {

class LootMessageConsumer : public AbstractMessageConsumer
{
  public:
  LootMessageConsumer(SystemServiceShPtr systemService, IMessageQueue *const messageQueue);
  ~LootMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  SystemServiceShPtr m_systemService{};
  IMessageQueue *const m_messageQueue{};
};

} // namespace bsgo
