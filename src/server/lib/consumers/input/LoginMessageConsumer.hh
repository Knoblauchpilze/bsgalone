
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "LoadingHelper.hh"
#include "LoginMessage.hh"
#include "LoginService.hh"
#include "SystemQueues.hh"

namespace bsgo {

class LoginMessageConsumer : public AbstractMessageConsumer
{
  public:
  LoginMessageConsumer(LoginServicePtr loginService,
                       SystemQueueMap systemQueues,
                       IMessageQueue *const outputMessageQueue);
  ~LoginMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  LoginServicePtr m_loginService{};
  IMessageQueue *const m_outputMessageQueue{};
  LoadingHelper m_helper;

  void handleLogin(const LoginMessage &message) const;
};

} // namespace bsgo
