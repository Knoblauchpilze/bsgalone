
#pragma once

#include "AbstractMessageConsumer.hh"
#include "ComponentSyncMessage.hh"
#include "IMessageQueue.hh"
#include "SystemQueues.hh"
#include "SystemService.hh"
#include "TargetMessage.hh"
#include <unordered_map>

namespace bsgo {

class RoutingMessageConsumer : public AbstractMessageConsumer
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
                         bsgalone::core::IMessageQueue *const outputMessageQueue);
  ~RoutingMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  SystemServiceShPtr m_systemService{};
  bsgalone::core::IMessageQueue *const m_outputMessageQueue{};

  void handleComponentSyncMessage(const ComponentSyncMessage &message) const;
  void handleTargetMessage(const TargetMessage &message) const;

  auto determineSystemForShip(const Uuid shipDbId) const -> Uuid;
  auto determineSystemForAsteroid(const Uuid asteroidDbId) const -> Uuid;
  auto determineSystemForOutpost(const Uuid outpostDbId) const -> Uuid;
};

} // namespace bsgo
