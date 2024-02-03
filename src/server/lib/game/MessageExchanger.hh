
#pragma once

#include "ClientManager.hh"
#include "Connection.hh"
#include "IMessageQueue.hh"
#include "NetworkMessageQueue.hh"
#include "SystemProcessor.hh"
#include "Uuid.hh"
#include <memory>
#include <vector>

namespace bsgo {

class MessageExchanger
{
  public:
  MessageExchanger(const ClientManagerShPtr &clientManager,
                   const std::vector<SystemProcessorShPtr> &systemProcessors);

  auto getOutputMessageQueue() const -> IMessageQueue *;
  void registerConnection(const Uuid clientId, net::ConnectionShPtr connection);

  private:
  NetworkMessageQueuePtr m_inputMessageQueue{};
  IMessageQueuePtr m_outputMessageQueue{};

  void initialize(const ClientManagerShPtr &clientManager,
                  const std::vector<SystemProcessorShPtr> &systemProcessors);
  void initializeQueues(ClientManagerShPtr clientManager);
  void initializeConsumers(const ClientManagerShPtr &clientManager,
                           const std::vector<SystemProcessorShPtr> &systemProcessors);
};

using MessageExchangerPtr = std::unique_ptr<MessageExchanger>;

} // namespace bsgo
