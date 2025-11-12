
#pragma once

#include "AbstractMessageConsumer.hh"
#include "AiBehaviorSyncMessage.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "SlotComponentMessage.hh"
#include "WeaponComponentMessage.hh"

namespace pge {

class ComponentMessageConsumer : public bsgo::AbstractMessageConsumer
{
  public:
  ComponentMessageConsumer(const bsgo::DatabaseEntityMapper &entityMapper,
                           bsgo::CoordinatorShPtr coordinator);
  ~ComponentMessageConsumer() override = default;

  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  const bsgo::DatabaseEntityMapper &m_entityMapper;
  bsgo::CoordinatorShPtr m_coordinator{};

  void handleComputerSlotUpdated(const bsgo::SlotComponentMessage &message) const;
  void handleWeaponUpdated(const bsgo::WeaponComponentMessage &message) const;
  void handleAiBehaviorUpdated(const bsgo::AiBehaviorSyncMessage &message) const;
};

} // namespace pge
