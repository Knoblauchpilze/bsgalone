
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "JumpCancelledMessage.hh"
#include "Services.hh"

namespace bsgo {

class JumpCancelledMessageConsumer : public AbstractMessageConsumer
{
  public:
  JumpCancelledMessageConsumer(const Services &services,
                               bsgalone::core::IMessageQueue *const outputMessageQueue);
  ~JumpCancelledMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  JumpServiceShPtr m_jumpService{};
  bsgalone::core::IMessageQueue *const m_outputMessageQueue{};

  void handleJumpCancellation(const JumpCancelledMessage &message) const;
};

} // namespace bsgo
