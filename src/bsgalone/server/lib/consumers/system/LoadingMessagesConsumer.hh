
#pragma once

#include "AbstractMessageConsumer.hh"
#include "ForPublishingLoginData.hh"
#include "IMessageQueue.hh"
#include "LoadingFinishedMessage.hh"
#include "LoadingStartedMessage.hh"
#include "Services.hh"

namespace bsgalone::server {

class LoadingMessagesConsumer : public core::AbstractMessageConsumer
{
  public:
  LoadingMessagesConsumer(const Services &services, core::IMessageQueueShPtr outputMessageQueue);
  ~LoadingMessagesConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  LoadingServiceShPtr m_loadingService{};
  core::IMessageQueueShPtr m_outputMessageQueue{};
  core::ForPublishingLoginDataPtr m_sendLoginDataUseCase{};

  void handleLoadingStartedMessage(const core::LoadingStartedMessage &message) const;
  void forwardLoadingFinishedMessage(const core::LoadingFinishedMessage &message) const;

  void handleActiveShipChangedTransition(const core::LoadingStartedMessage &message) const;
  void handleDockTransition(const core::LoadingStartedMessage &message) const;
  void handleEquipTransition(const core::LoadingStartedMessage &message) const;
  void handleJumpTransition(const core::LoadingStartedMessage &message) const;
  void handleLoginTransition(const core::LoadingStartedMessage &message) const;
  void handlePurchaseTransition(const core::LoadingStartedMessage &message) const;
  void handleUndockTransition(const core::LoadingStartedMessage &message) const;

  void handleLoginDataLoading(const core::LoadingStartedMessage &message) const;
  void handleResourcesLoading(const core::LoadingStartedMessage &message) const;
  void handleWeaponsLoading(const core::LoadingStartedMessage &message) const;
  void handleComputersLoading(const core::LoadingStartedMessage &message) const;
  void handleShipsLoading(const core::LoadingStartedMessage &message) const;

  void handlePlayerResourcesLoading(const core::LoadingStartedMessage &message) const;
  void handlePlayerShipsLoading(const core::LoadingStartedMessage &message) const;
  void handlePlayerComputersLoading(const core::LoadingStartedMessage &message) const;
  void handlePlayerWeaponsLoading(const core::LoadingStartedMessage &message) const;
  void handleActiveShipLoading(const core::LoadingStartedMessage &message) const;

  void handleSystemPlayersLoading(const core::LoadingStartedMessage &message) const;
  void handleSystemAsteroidsLoading(const core::LoadingStartedMessage &message) const;
  void handleSystemOutpostsLoading(const core::LoadingStartedMessage &message) const;
  void handleSystemShipsLoading(const core::LoadingStartedMessage &message) const;
  void handleSystemTargetsLoading(const core::LoadingStartedMessage &message) const;
  void handleSystemTickLoading(const core::LoadingStartedMessage &message) const;
};

} // namespace bsgalone::server
