
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "LogoutMessage.hh"
#include "SystemQueues.hh"
#include "SystemService.hh"
#include <unordered_map>

namespace bsgalone::server {

class LogoutMessageConsumer : public core::AbstractMessageConsumer
{
  public:
  LogoutMessageConsumer(SystemServiceShPtr systemService,
                        SystemQueueMap systemQueues,
                        core::IMessageQueue *const outputMessageQueue);
  ~LogoutMessageConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  SystemQueueMap m_systemQueues{};
  SystemServiceShPtr m_systemService{};
  core::IMessageQueue *const m_outputMessageQueue{};

  void handleLogout(const core::LogoutMessage &message) const;
  void notifyClientAndCloseConnectionIfNeeded(const core::Uuid playerDbId,
                                              const core::LogoutMessage &message) const;
};

} // namespace bsgalone::server
