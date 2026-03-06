
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "SystemService.hh"

namespace bsgalone::server {

class AiBehaviorSyncMessageConsumer : public core::AbstractMessageConsumer
{
  public:
  AiBehaviorSyncMessageConsumer(SystemServiceShPtr systemService,
                                core::IMessageQueue *const outputMessageQueue);
  ~AiBehaviorSyncMessageConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  SystemServiceShPtr m_systemService{};
  core::IMessageQueue *const m_outputMessageQueue{};
};

} // namespace bsgalone::server
