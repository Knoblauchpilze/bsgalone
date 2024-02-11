
#pragma once

#include "AbstractMessageConsumer.hh"
#include "ClientManager.hh"
#include "CombatService.hh"
#include "IMessageQueue.hh"
#include "LogoutMessage.hh"
#include "SystemProcessor.hh"
#include <unordered_map>

namespace bsgo {

class LogoutMessageConsumer : public AbstractMessageConsumer
{
  public:
  LogoutMessageConsumer(ClientManagerShPtr clientManager,
                        CombatServiceShPtr combatService,
                        const std::vector<SystemProcessorShPtr> &systemProcessors,
                        IMessageQueue *const messageQueue);
  ~LogoutMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  ClientManagerShPtr m_clientManager{};
  std::unordered_map<Uuid, SystemProcessorShPtr> m_systemProcessors{};
  CombatServiceShPtr m_combatService{};
  IMessageQueue *const m_messageQueue{};

  void handleLogout(const LogoutMessage &message) const;
  void notifyClientAndCloseConnectionIfNeeded(const Uuid playerDbId,
                                              const LogoutMessage &message) const;
};

} // namespace bsgo
