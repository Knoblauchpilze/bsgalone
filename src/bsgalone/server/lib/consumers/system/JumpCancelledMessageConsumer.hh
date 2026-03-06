
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "JumpCancelledMessage.hh"
#include "Services.hh"

namespace bsgalone::server {

class JumpCancelledMessageConsumer : public core::AbstractMessageConsumer
{
  public:
  JumpCancelledMessageConsumer(const Services &services,
                               core::IMessageQueue *const outputMessageQueue);
  ~JumpCancelledMessageConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  JumpServiceShPtr m_jumpService{};
  core::IMessageQueue *const m_outputMessageQueue{};

  void handleJumpCancellation(const core::JumpCancelledMessage &message) const;
};

} // namespace bsgalone::server
