
#pragma once

#include "AbstractMessageConsumer.hh"
#include "CombatService.hh"
#include "EntityDiedMessage.hh"
#include "IMessageQueue.hh"
#include "SystemProcessor.hh"
#include <unordered_map>

namespace bsgo {

class EntityDiedMessageConsumer : public AbstractMessageConsumer
{
  public:
  EntityDiedMessageConsumer(CombatServiceShPtr combatService,
                            const std::vector<SystemProcessorShPtr> &systemProcessors,
                            IMessageQueue *const messageQueue);
  ~EntityDiedMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  CombatServiceShPtr m_combatService{};
  IMessageQueue *const m_messageQueue{};
  std::unordered_map<Uuid, SystemProcessorShPtr> m_systemProcessors{};

  void handleShipEntityDied(const Uuid shipDbId) const;
  void handleAsteroidEntityDied(const Uuid asteroidDbId) const;
};

} // namespace bsgo
