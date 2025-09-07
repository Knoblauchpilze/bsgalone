
#pragma once

#include "AbstractMessageConsumer.hh"
#include "ClientManager.hh"
#include "IMessageQueue.hh"
#include "LoadingHelper.hh"
#include "LoginMessage.hh"
#include "LoginService.hh"
#include "SystemProcessor.hh"

namespace bsgo {

class LoginMessageConsumer : public AbstractMessageConsumer
{
  public:
  LoginMessageConsumer(LoginServicePtr loginService,
                       ClientManagerShPtr clientManager,
                       SystemProcessorMap systemProcessors,
                       IMessageQueue *const outputMessageQueue);
  ~LoginMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  LoginServicePtr m_loginService{};
  ClientManagerShPtr m_clientManager{};
  IMessageQueue *const m_outputMessageQueue{};
  LoadingHelper m_helper;

  void handleLogin(const LoginMessage &message) const;
};

} // namespace bsgo
