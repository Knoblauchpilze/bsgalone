
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "LoginMessage.hh"
#include "Services.hh"

namespace bsgo {

class LoginMessageConsumer : public AbstractMessageConsumer
{
  public:
  LoginMessageConsumer(const Services &services, IMessageQueue *const messageQueue);
  ~LoginMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  LoginServiceShPtr m_loginService{};
  IMessageQueue *const m_messageQueue{};

  void handleLogin(const LoginMessage &message) const;
};

} // namespace bsgo
