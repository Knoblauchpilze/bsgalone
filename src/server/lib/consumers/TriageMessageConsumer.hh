
#pragma once

#include "AbstractMessageConsumer.hh"
#include "ClientManager.hh"
#include "IMessageQueue.hh"
#include "SystemProcessor.hh"

namespace bsgo {

class TriageMessageConsumer : public AbstractMessageConsumer
{
  public:
  TriageMessageConsumer(std::vector<SystemProcessorShPtr> systemProcessors,
                        ClientManagerShPtr clientManager,
                        IMessageQueuePtr systemMessageQueue);
  ~TriageMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  ClientManagerShPtr m_clientManager{};
  std::vector<SystemProcessorShPtr> m_systemProcessors{};
  IMessageQueuePtr m_systemQueue{};

  void handleSystemMessage(const IMessage &message) const;
  void triagePlayerMessage(const IMessage &message) const;
};

} // namespace bsgo
