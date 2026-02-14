
#pragma once

#include "AbstractGameMessageConsumer.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "PlayerShipData.hh"
#include "Uuid.hh"

namespace pge {

class PlayerShipListMessageConsumer : public AbstractGameMessageConsumer
{
  public:
  PlayerShipListMessageConsumer(bsgo::DatabaseEntityMapper &entityMapper,
                                bsgo::CoordinatorShPtr coordinator);
  ~PlayerShipListMessageConsumer() override = default;

  protected:
  void onMessageReceivedInternal(const bsgalone::core::IMessage &message) override;

  private:
  bsgo::DatabaseEntityMapper &m_entityMapper;
  bsgo::CoordinatorShPtr m_coordinator{};

  void registerShip(const bsgo::PlayerShipData &data) const;
};

} // namespace pge
