
#pragma once

#include "CoreObject.hh"
#include "ForExecutingSignup.hh"
#include "IMessageListener.hh"

namespace bsgalone::server {

class SignupRequestConsumer : public core::IMessageListener, public ::core::CoreObject
{
  public:
  SignupRequestConsumer(ForExecutingSignupPtr signupUseCase);
  ~SignupRequestConsumer() override = default;

  bool isEventRelevant(const core::MessageType &type) const override;
  void onEventReceived(const core::IMessage &message) override;

  private:
  ForExecutingSignupPtr m_useCase{};
};

} // namespace bsgalone::server
