
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "SystemQueues.hh"
#include "SystemService.hh"

namespace bsgalone::server {

class JumpMessageConsumer : public core::AbstractMessageConsumer
{
  public:
  JumpMessageConsumer(SystemServiceShPtr systemService,
                      SystemQueueMap systemQueues,
                      core::IMessageQueue *const outputMessageQueue);
  ~JumpMessageConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  SystemServiceShPtr m_systemService{};
  SystemQueueMap m_systemQueues{};
  core::IMessageQueue *const m_outputMessageQueue{};

  void handlePostJumpSystemMessages(const core::Uuid shipDbId,
                                    const core::Uuid sourceSystemDbId,
                                    const core::Uuid destinationSystemDbId);
  void handleLoadingMessages(const core::Uuid playerDbId, const core::Uuid destinationSystemDbId);
};

} // namespace bsgalone::server
