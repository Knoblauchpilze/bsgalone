
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "Services.hh"
#include "SignupMessage.hh"

namespace bsgo {

class SignupMessageConsumer : public AbstractMessageConsumer
{
  public:
  SignupMessageConsumer(const Services &services, IMessageQueue *const messageQueue);
  ~SignupMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  SignupServiceShPtr m_signupService{};
  IMessageQueue *const m_messageQueue{};

  void handleSignup(const SignupMessage &message) const;
};

} // namespace bsgo
