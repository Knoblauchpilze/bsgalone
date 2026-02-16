
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "SystemService.hh"

namespace bsgo {

class AiBehaviorSyncMessageConsumer : public bsgalone::core::AbstractMessageConsumer
{
  public:
  AiBehaviorSyncMessageConsumer(SystemServiceShPtr systemService,
                                bsgalone::core::IMessageQueue *const outputMessageQueue);
  ~AiBehaviorSyncMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  SystemServiceShPtr m_systemService{};
  bsgalone::core::IMessageQueue *const m_outputMessageQueue{};
};

} // namespace bsgo
