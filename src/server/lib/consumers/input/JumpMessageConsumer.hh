
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "SystemProcessor.hh"
#include "SystemService.hh"

namespace bsgo {

class JumpMessageConsumer : public AbstractMessageConsumer
{
  public:
  JumpMessageConsumer(SystemServiceShPtr systemService,
                      SystemProcessorMap systemProcessors,
                      IMessageQueue *const messageQueue);
  ~JumpMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  SystemServiceShPtr m_systemService{};
  SystemProcessorMap m_systemProcessors{};
  IMessageQueue *const m_messageQueue{};

  void handlePostJumpSystemMessages(const Uuid shipDbId,
                                    const Uuid sourceSystemDbId,
                                    const Uuid destinationSystemDbId);
};

} // namespace bsgo
