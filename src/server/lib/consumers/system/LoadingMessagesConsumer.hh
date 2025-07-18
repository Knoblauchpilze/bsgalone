
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"
#include "Services.hh"

namespace bsgo {

class LoadingMessagesConsumer : public AbstractMessageConsumer
{
  public:
  LoadingMessagesConsumer(const Services &services, IMessageQueue *const outputMessageQueue);
  ~LoadingMessagesConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  LoadingServiceShPtr m_loadingService{};
  IMessageQueue *const m_outputMessageQueue{};

  void handleLoadingStartedMessage(const LoadingStartedMessage &message) const;
  void forwardLoadingFinishedMessage(const LoadingFinishedMessage &message) const;

  void handleLoginDataLoading(const LoadingStartedMessage &message) const;
  void handleSystemsLoading(const LoadingStartedMessage &message) const;
  void handlePlayersLoading(const LoadingStartedMessage &message) const;
  void handleAsteroidsLoading(const LoadingStartedMessage &message) const;
  void handleOutpostsLoading(const LoadingStartedMessage &message) const;
  void handleShipsLoading(const LoadingStartedMessage &message) const;
};

} // namespace bsgo
