
#pragma once

#include "AbstractMessageConsumer.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "PlayerShipData.hh"
#include "Uuid.hh"

namespace pge {

class ShipListMessageConsumer : public bsgo::AbstractMessageConsumer
{
  public:
  ShipListMessageConsumer(bsgo::DatabaseEntityMapper &entityMapper,
                          bsgo::CoordinatorShPtr coordinator);
  ~ShipListMessageConsumer() override = default;

  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  bsgo::DatabaseEntityMapper &m_entityMapper;
  bsgo::CoordinatorShPtr m_coordinator{};

  void registerShip(const bsgo::PlayerShipData &data) const;
};

} // namespace pge
