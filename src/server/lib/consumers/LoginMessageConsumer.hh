
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "LoginCallback.hh"
#include "LoginMessage.hh"
#include "Services.hh"

namespace bsgo {

class LoginMessageConsumer : public AbstractMessageConsumer
{
  public:
  LoginMessageConsumer(const Services &services,
                       IMessageQueue *const messageQueue,
                       const PlayerLoginCallback &playerLoginCallback);
  ~LoginMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  LoginServiceShPtr m_loginService{};
  IMessageQueue *const m_messageQueue{};
  PlayerLoginCallback m_playerLoginCallback{};

  void handleLogin(const LoginMessage &message) const;
};

} // namespace bsgo
