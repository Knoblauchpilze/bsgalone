
#pragma once

#include "AbstractMessageConsumer.hh"
#include "ClientManager.hh"
#include "IMessageQueue.hh"
#include "LoadingHelper.hh"
#include "SignupMessage.hh"
#include "SignupService.hh"
#include "SystemProcessor.hh"

namespace bsgo {

class SignupMessageConsumer : public AbstractMessageConsumer
{
  public:
  SignupMessageConsumer(SignupServicePtr signupService,
                        ClientManagerShPtr clientManager,
                        SystemProcessorMap systemProcessors,
                        IMessageQueue *const outputMessageQueue);
  ~SignupMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  SignupServicePtr m_signupService{};
  ClientManagerShPtr m_clientManager{};
  IMessageQueue *const m_outputMessageQueue{};
  LoadingHelper m_helper;

  void handleSignup(const SignupMessage &message) const;
};

} // namespace bsgo
