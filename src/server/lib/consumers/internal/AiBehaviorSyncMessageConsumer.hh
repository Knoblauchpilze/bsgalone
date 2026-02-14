
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "SystemService.hh"

namespace bsgo {

class AiBehaviorSyncMessageConsumer : public AbstractMessageConsumer
{
  public:
  AiBehaviorSyncMessageConsumer(SystemServiceShPtr systemService,
                                IMessageQueue *const outputMessageQueue);
  ~AiBehaviorSyncMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  SystemServiceShPtr m_systemService{};
  IMessageQueue *const m_outputMessageQueue{};
};

} // namespace bsgo
