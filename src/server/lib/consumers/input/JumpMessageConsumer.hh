
#pragma once

#include "AbstractMessageConsumer.hh"
#include "ClientManager.hh"
#include "IMessageQueue.hh"
#include "SystemProcessor.hh"
#include "SystemService.hh"

namespace bsgo {

class JumpMessageConsumer : public AbstractMessageConsumer
{
  public:
  JumpMessageConsumer(SystemServiceShPtr systemService,
                      ClientManagerShPtr clientManager,
                      SystemProcessorMap systemProcessors,
                      IMessageQueue *const messageQueue);
  ~JumpMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  SystemServiceShPtr m_systemService{};
  ClientManagerShPtr m_clientManager{};
  SystemProcessorMap m_systemProcessors{};
  IMessageQueue *const m_messageQueue{};

  void handlePostJumpSystemMessages(const Uuid shipDbId,
                                    const Uuid sourceSystemDbId,
                                    const Uuid destinationSystemDbId);
};

} // namespace bsgo
