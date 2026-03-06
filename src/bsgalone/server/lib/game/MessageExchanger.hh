
#pragma once

#include "IMessageQueue.hh"
#include "INetworkServer.hh"
#include "Repositories.hh"
#include "ServerNetworkClient.hh"
#include "SystemQueues.hh"
#include "SystemService.hh"
#include "Uuid.hh"
#include <memory>

namespace bsgalone::server {

struct MessageSystemData
{
  ServerNetworkClientShPtr networkClient{};
  SystemQueueMap systemQueues{};
};

class MessageExchanger
{
  public:
  MessageExchanger(const MessageSystemData &messagesData);

  auto getInternalMessageQueue() const -> core::IMessageQueue *;

  private:
  core::IMessageQueueShPtr m_internalMessageQueue{};

  void initialize(const MessageSystemData &messagesData);
  void initializeInternalConsumers(const MessageSystemData &messagesData);
};

using MessageExchangerPtr = std::unique_ptr<MessageExchanger>;

} // namespace bsgalone::server
