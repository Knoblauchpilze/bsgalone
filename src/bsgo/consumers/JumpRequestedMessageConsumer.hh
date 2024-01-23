
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "Services.hh"

namespace bsgo {

class JumpRequestedMessageConsumer : public AbstractMessageConsumer
{
  public:
  JumpRequestedMessageConsumer(const Services &services, IMessageQueue *const messageQueue);
  ~JumpRequestedMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  JumpServiceShPtr m_jumpService{};
  IMessageQueue *const m_messageQueue{};

  void handleJumpRequest(const Uuid &shipDbId,
                         const Uuid &shipEntityId,
                         const Uuid &jumpSystem) const;
};

} // namespace bsgo
