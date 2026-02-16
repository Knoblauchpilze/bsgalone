
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "JumpRequestedMessage.hh"
#include "Services.hh"

namespace bsgo {

class JumpRequestedMessageConsumer : public bsgalone::core::AbstractMessageConsumer
{
  public:
  JumpRequestedMessageConsumer(const Services &services,
                               bsgalone::core::IMessageQueue *const outputMessageQueue);
  ~JumpRequestedMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  JumpServiceShPtr m_jumpService{};
  bsgalone::core::IMessageQueue *const m_outputMessageQueue{};

  void handleJumpRequest(const JumpRequestedMessage &message) const;
};

} // namespace bsgo
