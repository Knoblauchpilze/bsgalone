
#pragma once

#include "AbstractMessageConsumer.hh"
#include "CombatService.hh"
#include "EntityRemovedMessage.hh"
#include "IMessageQueue.hh"
#include "SystemProcessor.hh"
#include <unordered_map>

namespace bsgo {

class EntityRemovedMessageConsumer : public AbstractMessageConsumer
{
  public:
  EntityRemovedMessageConsumer(CombatServiceShPtr combatService,
                               const std::vector<SystemProcessorShPtr> &systemProcessors,
                               IMessageQueue *const messageQueue);
  ~EntityRemovedMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  CombatServiceShPtr m_combatService{};
  IMessageQueue *const m_messageQueue{};
  std::unordered_map<Uuid, SystemProcessorShPtr> m_systemProcessors{};

  void handleShipEntityRemoved(const Uuid shipDbId, const bool dead) const;
  void handleAsteroidEntityRemoved(const Uuid asteroidDbId, const bool dead) const;
};

} // namespace bsgo
