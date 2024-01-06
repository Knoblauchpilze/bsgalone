
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
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
  SignupServiceShPtr m_signupService{};
  IMessageQueue *const m_messageQueue{};

  void handleLogin(const std::string &name, const std::string &password) const;
  void handleSignup(const std::string &name,
                    const std::string &password,
                    const Faction &faction) const;
};

} // namespace bsgo
