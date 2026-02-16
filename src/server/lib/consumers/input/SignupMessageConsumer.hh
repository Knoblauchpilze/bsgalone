
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "SignupMessage.hh"
#include "SignupService.hh"

namespace bsgo {

class SignupMessageConsumer : public bsgalone::core::AbstractMessageConsumer
{
  public:
  SignupMessageConsumer(SignupServicePtr signupService,
                        bsgalone::core::IMessageQueue *const outputMessageQueue);
  ~SignupMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  SignupServicePtr m_signupService{};
  bsgalone::core::IMessageQueue *const m_outputMessageQueue{};

  void handleSignup(const SignupMessage &message) const;
};

} // namespace bsgo
