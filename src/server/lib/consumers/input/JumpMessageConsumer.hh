
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "JumpService.hh"
#include "SystemProcessor.hh"

namespace bsgo {

class JumpMessageConsumer : public AbstractMessageConsumer
{
  public:
  JumpMessageConsumer(JumpServicePtr jumpService,
                      SystemProcessorMap systemProcessors,
                      IMessageQueue *const messageQueue);
  ~JumpMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  JumpServicePtr m_jumpService{};
  SystemProcessorMap m_systemProcessors{};
  IMessageQueue *const m_messageQueue{};
};

} // namespace bsgo
