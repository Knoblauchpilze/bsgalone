
#pragma once

#include "AbstractMessageConsumer.hh"
#include "ClientManager.hh"
#include "IMessageQueue.hh"
#include "SystemQueues.hh"
#include "SystemService.hh"

namespace bsgo {

class JumpMessageConsumer : public AbstractMessageConsumer
{
  public:
  JumpMessageConsumer(SystemServiceShPtr systemService,
                      bsgalone::server::ClientManagerShPtr clientManager,
                      SystemQueueMap systemQueues,
                      IMessageQueue *const outputMessageQueue);
  ~JumpMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  SystemServiceShPtr m_systemService{};
  bsgalone::server::ClientManagerShPtr m_clientManager{};
  SystemQueueMap m_systemQueues{};
  IMessageQueue *const m_outputMessageQueue{};

  void handlePostJumpSystemMessages(const Uuid shipDbId,
                                    const Uuid sourceSystemDbId,
                                    const Uuid destinationSystemDbId);
  void handleLoadingMessages(const Uuid playerDbId,
                             const Uuid destinationSystemDbId,
                             const Uuid clientId);
};

} // namespace bsgo
