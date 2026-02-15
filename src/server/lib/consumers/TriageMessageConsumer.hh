
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
                        bsgalone::core::IMessageQueuePtr systemMessageQueue);
  ~TriageMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  bsgalone::server::ClientManagerShPtr m_clientManager{};
  SystemQueueMap m_systemQueues{};
  bsgalone::core::IMessageQueuePtr m_systemQueue{};

  bool discardMessageWithNoClient(const bsgalone::core::IMessage &message) const;
  void handleSystemMessage(const bsgalone::core::IMessage &message) const;
  void triagePlayerMessage(const bsgalone::core::IMessage &message) const;
  void broadcastMessage(const bsgalone::core::IMessage &message) const;
};

} // namespace bsgo
