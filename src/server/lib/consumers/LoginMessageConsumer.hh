
#pragma once

#include "AbstractMessageConsumer.hh"
#include "ClientManager.hh"
#include "IMessageQueue.hh"
#include "LoginMessage.hh"
#include "LoginService.hh"

namespace bsgo {

class LoginMessageConsumer : public AbstractMessageConsumer
{
  public:
  LoginMessageConsumer(LoginServicePtr loginService,
                       ClientManagerShPtr clientManager,
                       IMessageQueue *const messageQueue);
  ~LoginMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  LoginServicePtr m_loginService{};
  ClientManagerShPtr m_clientManager{};
  IMessageQueue *const m_messageQueue{};

  void handleLogin(const LoginMessage &message) const;
};

} // namespace bsgo
