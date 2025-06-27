
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "JumpRequestedMessage.hh"
#include "Services.hh"

namespace bsgo {

class JumpRequestedMessageConsumer : public AbstractMessageConsumer
{
  public:
  JumpRequestedMessageConsumer(const Services &services, IMessageQueue *const outputMessageQueue);
  ~JumpRequestedMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  JumpServiceShPtr m_jumpService{};
  IMessageQueue *const m_outputMessageQueue{};

  void handleJumpRequest(const JumpRequestedMessage &message) const;
};

} // namespace bsgo
