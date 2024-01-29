
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
  TriageMessageConsumer(const std::vector<SystemProcessorShPtr> &systemProcessors,
                        ClientManagerShPtr clientManager,
                        IMessageQueuePtr systemMessageQueue);
  ~TriageMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  ClientManagerShPtr m_clientManager{};
  std::unordered_map<Uuid, SystemProcessorShPtr> m_systemProcessors{};
  IMessageQueuePtr m_systemQueue{};

  bool discardMessageWithNoClient(const IMessage &message) const;
  void handleSystemMessage(const IMessage &message) const;
  void triagePlayerMessage(const IMessage &message) const;
  void broadcastMessage(const IMessage &message) const;
};

} // namespace bsgo
