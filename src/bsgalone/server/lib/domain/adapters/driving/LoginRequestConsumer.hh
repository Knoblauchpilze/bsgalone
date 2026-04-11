
#pragma once

#include "CoreObject.hh"
#include "ForExecutingLogin.hh"
#include "IMessageListener.hh"

namespace bsgalone::server {

class LoginRequestConsumer : public core::IMessageListener, public ::core::CoreObject
{
  public:
  explicit LoginRequestConsumer(ForExecutingLoginPtr loginUseCase);
  ~LoginRequestConsumer() override = default;

  bool isEventRelevant(const core::MessageType &type) const override;
  void onEventReceived(const core::IMessage &message) override;

  private:
  ForExecutingLoginPtr m_useCase{};
};

} // namespace bsgalone::server
