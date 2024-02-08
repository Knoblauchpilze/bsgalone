
#pragma once

#include "AbstractMessageConsumer.hh"
#include "EntityDiedMessage.hh"
#include "IMessageQueue.hh"

namespace bsgo {

class EntityDiedMessageConsumer : public AbstractMessageConsumer
{
  public:
  EntityDiedMessageConsumer(IMessageQueue *const messageQueue);
  ~EntityDiedMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  IMessageQueue *const m_messageQueue{};

  void handleShipEntityDied(const Uuid shipDbId) const;
  void handleAsteroidEntityDied(const Uuid asteroidDbId) const;
};

} // namespace bsgo
