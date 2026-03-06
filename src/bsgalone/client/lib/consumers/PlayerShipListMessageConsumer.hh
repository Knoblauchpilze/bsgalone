
#pragma once

#include "AbstractGameMessageConsumer.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "PlayerShipData.hh"
#include "Uuid.hh"

namespace bsgalone::client {

class PlayerShipListMessageConsumer : public AbstractGameMessageConsumer
{
  public:
  PlayerShipListMessageConsumer(core::DatabaseEntityMapper &entityMapper,
                                core::CoordinatorShPtr coordinator);
  ~PlayerShipListMessageConsumer() override = default;

  protected:
  void onMessageReceivedInternal(const core::IMessage &message) override;

  private:
  core::DatabaseEntityMapper &m_entityMapper;
  core::CoordinatorShPtr m_coordinator{};

  void registerShip(const core::PlayerShipData &data) const;
};

} // namespace bsgalone::client
