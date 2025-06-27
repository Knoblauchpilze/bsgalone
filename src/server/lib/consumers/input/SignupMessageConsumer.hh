
#pragma once

#include "AbstractMessageConsumer.hh"
#include "ClientManager.hh"
#include "IMessageQueue.hh"
#include "SignupMessage.hh"
#include "SignupService.hh"

namespace bsgo {

class SignupMessageConsumer : public AbstractMessageConsumer
{
  public:
  SignupMessageConsumer(SignupServicePtr signupService,
                        ClientManagerShPtr clientManager,
                        IMessageQueue *const outputMessageQueue);
  ~SignupMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  SignupServicePtr m_signupService{};
  ClientManagerShPtr m_clientManager{};
  IMessageQueue *const m_outputMessageQueue{};

  void handleSignup(const SignupMessage &message) const;
};

} // namespace bsgo
