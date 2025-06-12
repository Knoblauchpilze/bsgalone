
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "LoadingFinishedMessage.hh"
#include "PlayerListMessage.hh"
#include "Services.hh"

namespace bsgo {

class LoadingMessagesConsumer : public AbstractMessageConsumer
{
  public:
  LoadingMessagesConsumer(const Services &services, IMessageQueue *const messageQueue);
  ~LoadingMessagesConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  IMessageQueue *const m_messageQueue{};

  void handlePlayersLoading(const PlayerListMessage &message) const;
  void forwardLoadingFinishedMessage(const LoadingFinishedMessage &message) const;
};

} // namespace bsgo
