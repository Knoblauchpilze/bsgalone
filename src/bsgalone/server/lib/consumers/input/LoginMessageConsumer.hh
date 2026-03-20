
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
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
  SystemQueueMap m_systemQueues{};
  core::IMessageQueue *const m_outputMessageQueue{};

  void publishLoadingMessages(const core::Uuid clientId,
                              const core::Uuid playerDbId,
                              const core::Uuid systemDbId) const;
};

} // namespace bsgalone::server
