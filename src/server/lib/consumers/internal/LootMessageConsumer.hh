
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "SystemService.hh"

namespace bsgo {

class LootMessageConsumer : public AbstractMessageConsumer
{
  public:
  LootMessageConsumer(SystemServiceShPtr systemService, IMessageQueue *const outputMessageQueue);
  ~LootMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  SystemServiceShPtr m_systemService{};
  IMessageQueue *const m_outputMessageQueue{};
};

} // namespace bsgo
