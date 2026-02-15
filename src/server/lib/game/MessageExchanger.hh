
#pragma once

#include "ClientManager.hh"
#include "IEventBus.hh"
#include "IMessageQueue.hh"
#include "INetworkServer.hh"
#include "Repositories.hh"
#include "ServerNetworkClient.hh"
#include "SystemQueues.hh"
#include "SystemService.hh"
#include "Uuid.hh"
#include <memory>

namespace bsgo {

struct MessageSystemData
{
  bsgalone::server::ClientManagerShPtr clientManager{};
  ServerNetworkClientShPtr networkClient{};
  SystemQueueMap systemQueues{};
};

class MessageExchanger
{
  public:
  MessageExchanger(const MessageSystemData &messagesData);

  auto getInternalMessageQueue() const -> bsgalone::core::IMessageQueue *;

  private:
  bsgalone::core::IMessageQueueShPtr m_internalMessageQueue{};

  void initialize(const MessageSystemData &messagesData);
  auto initializeSystemMessageQueue(const MessageSystemData &messagesData)
    -> bsgalone::core::IMessageQueuePtr;
  void initializeInternalConsumers(const MessageSystemData &messagesData);
};

using MessageExchangerPtr = std::unique_ptr<MessageExchanger>;

} // namespace bsgo
