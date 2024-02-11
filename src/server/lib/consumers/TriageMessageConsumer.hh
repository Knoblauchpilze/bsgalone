
#pragma once

#include "AbstractMessageConsumer.hh"
#include "ClientManager.hh"
#include "IMessageQueue.hh"
#include "SystemProcessor.hh"
#include <unordered_map>

namespace bsgo {

class TriageMessageConsumer : public AbstractMessageConsumer
{
  public:
  TriageMessageConsumer(ClientManagerShPtr clientManager,
                        SystemProcessorMap systemProcessors,
                        IMessageQueuePtr systemMessageQueue);
  ~TriageMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  ClientManagerShPtr m_clientManager{};
  SystemProcessorMap m_systemProcessors{};
  IMessageQueuePtr m_systemQueue{};

  bool discardMessageWithNoClient(const IMessage &message) const;
  void handleSystemMessage(const IMessage &message) const;
  void triagePlayerMessage(const IMessage &message) const;
  void broadcastMessage(const IMessage &message) const;
};

} // namespace bsgo
