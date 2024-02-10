
#pragma once

#include "AbstractMessageConsumer.hh"
#include "ClientManager.hh"
#include "IMessageQueue.hh"
#include "LogoutMessage.hh"

namespace bsgo {

class LogoutMessageConsumer : public AbstractMessageConsumer
{
  public:
  LogoutMessageConsumer(ClientManagerShPtr clientManager, IMessageQueue *const messageQueue);
  ~LogoutMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  ClientManagerShPtr m_clientManager{};
  IMessageQueue *const m_messageQueue{};

  void handleLogout(const LogoutMessage &message) const;
};

} // namespace bsgo
