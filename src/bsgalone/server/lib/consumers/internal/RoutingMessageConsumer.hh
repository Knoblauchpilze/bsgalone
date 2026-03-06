
#pragma once

#include "AbstractMessageConsumer.hh"
#include "ComponentSyncMessage.hh"
#include "IMessageQueue.hh"
#include "SystemQueues.hh"
#include "SystemService.hh"
#include "TargetMessage.hh"
#include <unordered_map>

namespace bsgalone::server {

class RoutingMessageConsumer : public core::AbstractMessageConsumer
{
  public:
  /// @brief - Consumer in charge or receiving internal messages produced by the
  /// system processors and attaching the correct system identifier to messages
  /// so that they can be routed correctly by the broadcast message queue.
  /// This is meant as a generic routing component in case a specific message does
  /// not need complex processing but only routing.
  /// @param systemService - service to use to get information about entities and
  /// in which system they belong
  /// @param outputMessageQueue - the message queue to push messages to the client
  /// applications
  RoutingMessageConsumer(SystemServiceShPtr systemService,
                         core::IMessageQueue *const outputMessageQueue);
  ~RoutingMessageConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  SystemServiceShPtr m_systemService{};
  core::IMessageQueue *const m_outputMessageQueue{};

  void handleComponentSyncMessage(const core::ComponentSyncMessage &message) const;
  void handleTargetMessage(const core::TargetMessage &message) const;

  auto determineSystemForShip(const core::Uuid shipDbId) const -> core::Uuid;
  auto determineSystemForAsteroid(const core::Uuid asteroidDbId) const -> core::Uuid;
  auto determineSystemForOutpost(const core::Uuid outpostDbId) const -> core::Uuid;
};

} // namespace bsgalone::server
