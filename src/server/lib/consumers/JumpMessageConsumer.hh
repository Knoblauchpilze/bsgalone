
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "JumpMessage.hh"
#include "Services.hh"

namespace bsgo {

class JumpMessageConsumer : public AbstractMessageConsumer
{
  public:
  JumpMessageConsumer(const Services &services, IMessageQueue *const messageQueue);
  ~JumpMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  JumpServiceShPtr m_jumpService{};
  IMessageQueue *const m_messageQueue{};

  void handleJump(const JumpMessage &message) const;
};

} // namespace bsgo
