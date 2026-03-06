
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "SignupMessage.hh"
#include "SignupService.hh"

namespace bsgalone::server {

class SignupMessageConsumer : public core::AbstractMessageConsumer
{
  public:
  SignupMessageConsumer(SignupServicePtr signupService,
                        core::IMessageQueue *const outputMessageQueue);
  ~SignupMessageConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  SignupServicePtr m_signupService{};
  core::IMessageQueue *const m_outputMessageQueue{};

  void handleSignup(const core::SignupMessage &message) const;
};

} // namespace bsgalone::server
