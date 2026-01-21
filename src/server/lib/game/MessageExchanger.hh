
#pragma once

#include "ClientManager.hh"
#include "Connection.hh"
#include "IEventBus.hh"
#include "IMessageQueue.hh"
#include "INetworkServer.hh"
#include "Repositories.hh"
#include "SystemProcessor.hh"
#include "SystemService.hh"
#include "Uuid.hh"
#include <memory>

namespace bsgo {

struct MessageSystemData
{
  ClientManagerShPtr clientManager{};
  net::INetworkServerShPtr server{};
  SystemProcessorMap systemProcessors{};
};

class MessageExchanger
{
  public:
  MessageExchanger(const MessageSystemData &messagesData);

  auto getInputMessageQueue() const -> IMessageQueueShPtr;
  auto getInternalMessageQueue() const -> IMessageQueue *;
  auto getOutputMessageQueue() const -> IMessageQueue *;

  private:
  IMessageQueueShPtr m_inputMessageQueue{};
  IMessageQueuePtr m_outputMessageQueue{};
  IMessageQueuePtr m_internalMessageQueue{};

  void initialize(const MessageSystemData &messagesData);
  auto initializeSystemMessageQueue(const MessageSystemData &messagesData) -> IMessageQueuePtr;
  void initializeInternalConsumers(const MessageSystemData &messagesData);
};

using MessageExchangerPtr = std::unique_ptr<MessageExchanger>;

} // namespace bsgo
