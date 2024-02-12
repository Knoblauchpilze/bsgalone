
#pragma once

#include "AbstractMessageConsumer.hh"
#include "Services.hh"

namespace bsgo {

class EntityDeletedMessageConsumer : public AbstractMessageConsumer
{
  public:
  EntityDeletedMessageConsumer(const Services &services);
  ~EntityDeletedMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  ShipServiceShPtr m_shipService{};

  void handleShipRemoved(const Uuid shipDbId) const;
  void handleAsteroidRemoved(const Uuid asteroidDbId) const;
};

} // namespace bsgo
