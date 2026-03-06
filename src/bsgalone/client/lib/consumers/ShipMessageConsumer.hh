
#pragma once

#include "AbstractMessageConsumer.hh"
#include "ComponentSyncMessage.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "JumpCancelledMessage.hh"
#include "JumpRequestedMessage.hh"
#include "TargetMessage.hh"

namespace bsgalone::client {

class ShipMessageConsumer : public core::AbstractMessageConsumer
{
  public:
  ShipMessageConsumer(const core::DatabaseEntityMapper &entityMapper,
                      core::CoordinatorShPtr coordinator);
  ~ShipMessageConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  const core::DatabaseEntityMapper &m_entityMapper;
  core::CoordinatorShPtr m_coordinator{};

  void handleComponentSync(const core::ComponentSyncMessage &message) const;
  void handleJumpRequested(const core::JumpRequestedMessage &message) const;
  void handleJumpCancelled(const core::JumpCancelledMessage &message) const;
  void handleTargetAcquired(const core::TargetMessage &message) const;

  void handleShipComponentsSync(const core::ComponentSyncMessage &message) const;
  void handleAsteroidComponentsSync(const core::ComponentSyncMessage &message) const;
  void handleOutpostComponentsSync(const core::ComponentSyncMessage &message) const;
};

} // namespace bsgalone::client
