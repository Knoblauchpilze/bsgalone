
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "SystemQueues.hh"
#include "SystemService.hh"

namespace bsgo {

class JumpMessageConsumer : public AbstractMessageConsumer
{
  public:
  JumpMessageConsumer(SystemServiceShPtr systemService,
                      SystemQueueMap systemQueues,
                      IMessageQueue *const outputMessageQueue);
  ~JumpMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  SystemServiceShPtr m_systemService{};
  SystemQueueMap m_systemQueues{};
  IMessageQueue *const m_outputMessageQueue{};

  void handlePostJumpSystemMessages(const Uuid shipDbId,
                                    const Uuid sourceSystemDbId,
                                    const Uuid destinationSystemDbId);
  void handleLoadingMessages(const Uuid playerDbId, const Uuid destinationSystemDbId);
};

} // namespace bsgo
