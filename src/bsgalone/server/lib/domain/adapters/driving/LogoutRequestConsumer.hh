
#pragma once

#include "CoreObject.hh"
#include "ForExecutingLogout.hh"
#include "IMessageListener.hh"

namespace bsgalone::server {

class LogoutRequestConsumer : public core::IMessageListener, public ::core::CoreObject
{
  public:
  LogoutRequestConsumer(ForExecutingLogoutPtr logoutUseCase);
  ~LogoutRequestConsumer() override = default;

  bool isEventRelevant(const core::MessageType &type) const override;
  void onEventReceived(const core::IMessage &message) override;

  private:
  ForExecutingLogoutPtr m_useCase{};
};

} // namespace bsgalone::server
