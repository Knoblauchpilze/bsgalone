
#pragma once

#include "AbstractMessageConsumer.hh"
#include "ComponentSyncMessage.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "JumpCancelledMessage.hh"
#include "JumpRequestedMessage.hh"
#include "TargetMessage.hh"

namespace pge {

class ShipMessageConsumer : public bsgalone::core::AbstractMessageConsumer
{
  public:
  ShipMessageConsumer(const bsgo::DatabaseEntityMapper &entityMapper,
                      bsgo::CoordinatorShPtr coordinator);
  ~ShipMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  const bsgo::DatabaseEntityMapper &m_entityMapper;
  bsgo::CoordinatorShPtr m_coordinator{};

  void handleComponentSync(const bsgo::ComponentSyncMessage &message) const;
  void handleJumpRequested(const bsgalone::core::JumpRequestedMessage &message) const;
  void handleJumpCancelled(const bsgalone::core::JumpCancelledMessage &message) const;
  void handleTargetAcquired(const bsgo::TargetMessage &message) const;

  void handleShipComponentsSync(const bsgo::ComponentSyncMessage &message) const;
  void handleAsteroidComponentsSync(const bsgo::ComponentSyncMessage &message) const;
  void handleOutpostComponentsSync(const bsgo::ComponentSyncMessage &message) const;
};

} // namespace pge
