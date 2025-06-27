
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "SystemProcessor.hh"
#include "SystemService.hh"
#include <unordered_map>

namespace bsgo {

class EntityRemovedMessageConsumer : public AbstractMessageConsumer
{
  public:
  EntityRemovedMessageConsumer(SystemServiceShPtr systemService,
                               SystemProcessorMap systemProcessors,
                               IMessageQueue *const outputMessageQueue);
  ~EntityRemovedMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  SystemServiceShPtr m_systemService{};
  SystemProcessorMap m_systemProcessors{};
  IMessageQueue *const m_outputMessageQueue{};

  void handleShipEntityRemoved(const Uuid shipDbId, const bool dead) const;
  void handleAsteroidEntityRemoved(const Uuid asteroidDbId, const bool dead) const;
};

} // namespace bsgo
