
#pragma once

#include "AbstractMessageConsumer.hh"
#include "CombatService.hh"
#include "EntityDiedMessage.hh"
#include "IMessageQueue.hh"

namespace bsgo {

class EntityDiedMessageConsumer : public AbstractMessageConsumer
{
  public:
  EntityDiedMessageConsumer(CombatServiceShPtr combatService, IMessageQueue *const messageQueue);
  ~EntityDiedMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  CombatServiceShPtr m_combatService{};
  IMessageQueue *const m_messageQueue{};

  void handleShipEntityDied(const Uuid shipDbId) const;
  void handleAsteroidEntityDied(const Uuid asteroidDbId) const;
};

} // namespace bsgo
