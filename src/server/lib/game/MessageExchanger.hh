
#pragma once

#include "ClientManager.hh"
#include "Connection.hh"
#include "IMessageQueue.hh"
#include "NetworkMessageQueue.hh"
#include "Repositories.hh"
#include "SystemQueues.hh"
#include "SystemService.hh"
#include "Uuid.hh"
#include <memory>

namespace bsgo {

struct MessageSystemData
{
  ClientManagerShPtr clientManager{};
  SystemQueueMap systemQueues{};
};

class MessageExchanger
{
  public:
  MessageExchanger(const MessageSystemData &messagesData);

  auto getInternalMessageQueue() const -> IMessageQueue *;
  auto getOutputMessageQueue() const -> IMessageQueue *;
  void registerConnection(net::ConnectionShPtr connection);

  void pushMessage(IMessagePtr message);

  private:
  IMessageQueuePtr m_outputMessageQueue{};
  IMessageQueuePtr m_internalMessageQueue{};
  NetworkMessageQueuePtr m_inputMessageQueue{};

  void initialize(const MessageSystemData &messagesData);
  auto initializeSystemMessageQueue(const MessageSystemData &messagesData) -> IMessageQueuePtr;
  void initializeInternalConsumers(const MessageSystemData &messagesData);
};

using MessageExchangerPtr = std::unique_ptr<MessageExchanger>;

} // namespace bsgo
