
#pragma once

#include "AbstractMessageConsumer.hh"
#include "AiBehaviorSyncMessage.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "SlotComponentMessage.hh"
#include "WeaponComponentMessage.hh"

namespace bsgalone::client {

class ComponentMessageConsumer : public core::AbstractMessageConsumer
{
  public:
  ComponentMessageConsumer(const core::DatabaseEntityMapper &entityMapper,
                           core::CoordinatorShPtr coordinator);
  ~ComponentMessageConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  const core::DatabaseEntityMapper &m_entityMapper;
  core::CoordinatorShPtr m_coordinator{};

  void handleComputerSlotUpdated(const core::SlotComponentMessage &message) const;
  void handleWeaponUpdated(const core::WeaponComponentMessage &message) const;
  void handleAiBehaviorUpdated(const core::AiBehaviorSyncMessage &message) const;
};

} // namespace bsgalone::client
