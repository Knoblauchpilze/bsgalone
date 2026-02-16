
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "LoadingHelper.hh"
#include "LoginMessage.hh"
#include "LoginService.hh"
#include "SystemQueues.hh"

namespace bsgo {

class LoginMessageConsumer : public bsgalone::core::AbstractMessageConsumer
{
  public:
  LoginMessageConsumer(LoginServicePtr loginService,
                       SystemQueueMap systemQueues,
                       bsgalone::core::IMessageQueue *const outputMessageQueue);
  ~LoginMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  LoginServicePtr m_loginService{};
  bsgalone::core::IMessageQueue *const m_outputMessageQueue{};
  LoadingHelper m_helper;

  void handleLogin(const LoginMessage &message) const;
};

} // namespace bsgo
