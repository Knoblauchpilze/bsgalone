
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "LogoutMessage.hh"
#include "SystemQueues.hh"
#include "SystemService.hh"
#include <unordered_map>

namespace bsgo {

class LogoutMessageConsumer : public bsgalone::core::AbstractMessageConsumer
{
  public:
  LogoutMessageConsumer(SystemServiceShPtr systemService,
                        SystemQueueMap systemQueues,
                        bsgalone::core::IMessageQueue *const outputMessageQueue);
  ~LogoutMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  SystemQueueMap m_systemQueues{};
  SystemServiceShPtr m_systemService{};
  bsgalone::core::IMessageQueue *const m_outputMessageQueue{};

  void handleLogout(const LogoutMessage &message) const;
  void notifyClientAndCloseConnectionIfNeeded(const Uuid playerDbId,
                                              const LogoutMessage &message) const;
};

} // namespace bsgo
