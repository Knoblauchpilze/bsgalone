
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "SystemQueues.hh"
#include "SystemService.hh"
#include <unordered_map>

namespace bsgo {

/// @brief - This consumer is added to the internal consumers and is responsible
/// to process the entity added/removed messages produced by the systems in the
/// processors.
/// This processing only includes:
///   - sending ships back to the outpost
///   - forwarding messages for asteroids
/// The messages are then routed to the processors to be processed like a regular
/// entity added/removed message.
class EntityRemovedMessageConsumer : public AbstractMessageConsumer
{
  public:
  EntityRemovedMessageConsumer(SystemServiceShPtr systemService,
                               SystemQueueMap systemQueues,
                               bsgalone::core::IMessageQueue *const outputMessageQueue);
  ~EntityRemovedMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  SystemServiceShPtr m_systemService{};
  SystemQueueMap m_systemQueues{};
  bsgalone::core::IMessageQueue *const m_outputMessageQueue{};

  void handleShipEntityRemoved(const Uuid shipDbId, const bool dead) const;
  void handleAsteroidEntityRemoved(const Uuid asteroidDbId, const bool dead) const;
};

} // namespace bsgo
