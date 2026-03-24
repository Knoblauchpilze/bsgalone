
#pragma once

#include "AbstractMessageConsumer.hh"
#include "ForExecutingLogin.hh"
#include "ForPublishingEvent.hh"
#include "IMessageQueue.hh"
#include "LoginService.hh"
#include "SystemQueues.hh"

namespace bsgalone::server {

class LoginRequestConsumer : public core::AbstractMessageConsumer
{
  public:
  LoginRequestConsumer(LoginServicePtr loginService,
                       SystemQueueMap systemQueues,
                       core::IMessageQueue *const outputMessageQueue,
                       core::ForPublishingEventShPtr gameEventPublisher);
  ~LoginRequestConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  LoginServicePtr m_loginService{};
  SystemQueueMap m_systemQueues{};
  core::IMessageQueue *const m_outputMessageQueue{};

  core::ForExecutingLoginPtr m_executeLoginUseCase{};

  void publishLoadingMessages(const core::Uuid clientId,
                              const core::Uuid playerDbId,
                              const core::Uuid systemDbId) const;
};

} // namespace bsgalone::server
