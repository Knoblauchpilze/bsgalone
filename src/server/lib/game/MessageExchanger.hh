
#pragma once

#include "ClientManager.hh"
#include "Connection.hh"
#include "IEventBus.hh"
#include "IMessageQueue.hh"
#include "NetworkMessageQueue.hh"
#include "Repositories.hh"
#include "SystemProcessor.hh"
#include "SystemService.hh"
#include "Uuid.hh"
#include <memory>

namespace bsgo {

struct MessageSystemData
{
  net::IEventBus &eventBus;
  ClientManagerShPtr clientManager{};
  SystemProcessorMap systemProcessors{};
};

class MessageExchanger
{
  public:
  MessageExchanger(const MessageSystemData &messagesData);

  auto getInternalMessageQueue() const -> IMessageQueue *;
  auto getOutputMessageQueue() const -> IMessageQueue *;

  private:
  IMessageQueuePtr m_outputMessageQueue{};
  IMessageQueuePtr m_internalMessageQueue{};

  void initialize(const MessageSystemData &messagesData);
  auto initializeSystemMessageQueue(const MessageSystemData &messagesData) -> IMessageQueuePtr;
  void initializeInternalConsumers(const MessageSystemData &messagesData);
};

using MessageExchangerPtr = std::unique_ptr<MessageExchanger>;

} // namespace bsgo
