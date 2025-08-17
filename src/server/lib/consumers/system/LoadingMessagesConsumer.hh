
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

  void handleActiveShipChangedTransition(const LoadingStartedMessage &message) const;
  void handleDockTransition(const LoadingStartedMessage &message) const;
  void handleEquipTransition(const LoadingStartedMessage &message) const;
  void handleJumpTransition(const LoadingStartedMessage &message) const;
  void handleLoginTransition(const LoadingStartedMessage &message) const;
  void handlePurchaseTransition(const LoadingStartedMessage &message) const;
  void handleUndockTransition(const LoadingStartedMessage &message) const;

  void handleLoginDataLoading(const LoadingStartedMessage &message) const;
  void handleResourcesLoading(const LoadingStartedMessage &message) const;
  void handleSystemsLoading(const LoadingStartedMessage &message) const;

  void handlePlayerResourcesLoading(const LoadingStartedMessage &message) const;
  void handlePlayerShipsLoading(const LoadingStartedMessage &message) const;
  void handlePlayerComputersLoading(const LoadingStartedMessage &message) const;
  void handlePlayerWeaponsLoading(const LoadingStartedMessage &message) const;
  void handleActiveShipLoading(const LoadingStartedMessage &message) const;

  void handleSystemPlayersLoading(const LoadingStartedMessage &message) const;
  void handleSystemAsteroidsLoading(const LoadingStartedMessage &message) const;
  void handleSystemOutpostsLoading(const LoadingStartedMessage &message) const;
  void handleSystemShipsLoading(const LoadingStartedMessage &message) const;
};

} // namespace bsgo
