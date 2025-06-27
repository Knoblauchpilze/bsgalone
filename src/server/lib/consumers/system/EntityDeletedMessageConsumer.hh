
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "Services.hh"

namespace bsgo {

class EntityDeletedMessageConsumer : public AbstractMessageConsumer
{
  public:
  EntityDeletedMessageConsumer(const Services &services, IMessageQueue *const outputMessageQueue);
  ~EntityDeletedMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  EntityServiceShPtr m_entityService{};
  IMessageQueue *const m_outputMessageQueue{};

  void handleShipRemoved(const Uuid shipDbId) const;
  void handleAsteroidRemoved(const Uuid asteroidDbId) const;
};

} // namespace bsgo
