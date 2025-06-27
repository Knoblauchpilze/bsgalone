
#pragma once

#include "AbstractMessageConsumer.hh"
#include "ClientManager.hh"
#include "IMessageQueue.hh"
#include "LogoutMessage.hh"
#include "SystemProcessor.hh"
#include "SystemService.hh"
#include <unordered_map>

namespace bsgo {

class LogoutMessageConsumer : public AbstractMessageConsumer
{
  public:
  LogoutMessageConsumer(ClientManagerShPtr clientManager,
                        SystemServiceShPtr systemService,
                        SystemProcessorMap systemProcessors,
                        IMessageQueue *const outputMessageQueue);
  ~LogoutMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  ClientManagerShPtr m_clientManager{};
  SystemProcessorMap m_systemProcessors{};
  SystemServiceShPtr m_systemService{};
  IMessageQueue *const m_outputMessageQueue{};

  void handleLogout(const LogoutMessage &message) const;
  void notifyClientAndCloseConnectionIfNeeded(const Uuid playerDbId,
                                              const LogoutMessage &message) const;
};

} // namespace bsgo
