
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "Services.hh"

namespace bsgo {

class JumpCancelledMessageConsumer : public AbstractMessageConsumer
{
  public:
  JumpCancelledMessageConsumer(const Services &services, IMessageQueue *const messageQueue);
  ~JumpCancelledMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  JumpServiceShPtr m_jumpService{};
  IMessageQueue *const m_messageQueue{};

  void handleJumpCancellation(const Uuid &shipDbId, const Uuid &shipEntityId) const;
};

} // namespace bsgo
