
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
  ClientManagerShPtr clientManager{};
  ServerNetworkClientShPtr networkClient{};
  SystemQueueMap systemQueues{};
};

class MessageExchanger
{
  public:
  MessageExchanger(const MessageSystemData &messagesData);

  auto getInternalMessageQueue() const -> IMessageQueue *;

  private:
  IMessageQueuePtr m_internalMessageQueue{};

  void initialize(const MessageSystemData &messagesData);
  auto initializeSystemMessageQueue(const MessageSystemData &messagesData) -> IMessageQueuePtr;
  void initializeInternalConsumers(const MessageSystemData &messagesData);
};

using MessageExchangerPtr = std::unique_ptr<MessageExchanger>;

} // namespace bsgo
