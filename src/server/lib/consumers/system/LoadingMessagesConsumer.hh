
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"
#include "PlayerListMessage.hh"
#include "Services.hh"

namespace bsgo {

class LoadingMessagesConsumer : public AbstractMessageConsumer
{
  public:
  LoadingMessagesConsumer(const Services &services, IMessageQueue *const outputMessageQueue);
  ~LoadingMessagesConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  IMessageQueue *const m_outputMessageQueue{};

  void forwardLoadingStartedMessage(const LoadingStartedMessage &message) const;
  void forwardLoadingFinishedMessage(const LoadingFinishedMessage &message) const;
};

} // namespace bsgo
