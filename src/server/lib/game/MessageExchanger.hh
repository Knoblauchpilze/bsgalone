
#pragma once

#include "ClientManager.hh"
#include "Connection.hh"
#include "IMessageQueue.hh"
#include "NetworkMessageQueue.hh"
#include "Repositories.hh"
#include "SystemProcessor.hh"
#include "SystemService.hh"
#include "Uuid.hh"
#include <memory>

namespace bsgo {

class MessageExchanger
{
  public:
  MessageExchanger(const ClientManagerShPtr &clientManager,
                   const SystemProcessorMap &systemProcessors);

  auto getInternalMessageQueue() const -> IMessageQueue *;
  auto getOutputMessageQueue() const -> IMessageQueue *;
  void registerConnection(const Uuid clientId, net::ConnectionShPtr connection);

  void pushMessage(IMessagePtr message);

  private:
  NetworkMessageQueuePtr m_inputMessageQueue{};
  IMessageQueuePtr m_internalMessageQueue{};
  IMessageQueuePtr m_outputMessageQueue{};

  void initialize(const ClientManagerShPtr &clientManager,
                  const SystemProcessorMap &systemProcessors);
  void initializeQueues(ClientManagerShPtr clientManager);
  void initializeConsumers(const ClientManagerShPtr &clientManager,
                           const SystemProcessorMap &systemProcessors);

  void initializeInternalMessageQueue(const SystemServiceShPtr &systemService,
                                      const SystemProcessorMap &systemProcessors);
};

using MessageExchangerPtr = std::unique_ptr<MessageExchanger>;

} // namespace bsgo
