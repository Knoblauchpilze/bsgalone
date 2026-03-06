
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "JumpRequestedMessage.hh"
#include "Services.hh"

namespace bsgalone::server {

class JumpRequestedMessageConsumer : public core::AbstractMessageConsumer
{
  public:
  JumpRequestedMessageConsumer(const Services &services,
                               core::IMessageQueue *const outputMessageQueue);
  ~JumpRequestedMessageConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  JumpServiceShPtr m_jumpService{};
  core::IMessageQueue *const m_outputMessageQueue{};

  void handleJumpRequest(const core::JumpRequestedMessage &message) const;
};

} // namespace bsgalone::server
