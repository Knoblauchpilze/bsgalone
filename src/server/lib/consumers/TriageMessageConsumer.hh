
#pragma once

#include "AbstractMessageConsumer.hh"
#include "ClientManager.hh"
#include "IMessageQueue.hh"
#include "SystemQueues.hh"
#include <unordered_map>

namespace bsgo {

class TriageMessageConsumer : public AbstractMessageConsumer
{
  public:
  TriageMessageConsumer(bsgalone::server::ClientManagerShPtr clientManager,
                        SystemQueueMap systemQueues,
                        IMessageQueuePtr systemMessageQueue);
  ~TriageMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  bsgalone::server::ClientManagerShPtr m_clientManager{};
  SystemQueueMap m_systemQueues{};
  IMessageQueuePtr m_systemQueue{};

  bool discardMessageWithNoClient(const IMessage &message) const;
  void handleSystemMessage(const IMessage &message) const;
  void triagePlayerMessage(const IMessage &message) const;
  void broadcastMessage(const IMessage &message) const;
};

} // namespace bsgo
