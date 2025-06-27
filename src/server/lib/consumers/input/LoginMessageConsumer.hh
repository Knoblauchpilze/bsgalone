
#pragma once

#include "AbstractMessageConsumer.hh"
#include "ClientManager.hh"
#include "IMessageQueue.hh"
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
  SystemProcessorMap m_systemProcessors{};
  ClientManagerShPtr m_clientManager{};
  IMessageQueue *const m_outputMessageQueue{};

  void handleLogin(const LoginMessage &message) const;
  void publishLoadingMessages(const bsgo::Uuid clientId, const bsgo::Uuid playerDbId) const;
};

} // namespace bsgo
