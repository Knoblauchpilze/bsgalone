
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "LoadingHelper.hh"
#include "LoginService.hh"
#include "SystemQueues.hh"

namespace bsgalone::server {

class LoginMessageConsumer : public core::AbstractMessageConsumer
{
  public:
  LoginMessageConsumer(LoginServicePtr loginService,
                       SystemQueueMap systemQueues,
                       core::IMessageQueue *const outputMessageQueue);
  ~LoginMessageConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  LoginServicePtr m_loginService{};
  core::IMessageQueue *const m_outputMessageQueue{};
  LoadingHelper m_helper;
};

} // namespace bsgalone::server
