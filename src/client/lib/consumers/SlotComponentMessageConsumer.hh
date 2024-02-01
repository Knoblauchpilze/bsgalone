
#pragma once

#include "AbstractMessageConsumer.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "SlotComponentMessage.hh"

namespace pge {

class SlotComponentMessageConsumer : public bsgo::AbstractMessageConsumer
{
  public:
  SlotComponentMessageConsumer(const bsgo::DatabaseEntityMapper &entityMapper,
                               bsgo::CoordinatorShPtr coordinator);
  ~SlotComponentMessageConsumer() override = default;

  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  const bsgo::DatabaseEntityMapper &m_entityMapper;
  bsgo::CoordinatorShPtr m_coordinator{};

  void handleComputerSlotUpdated(const bsgo::SlotComponentMessage &message) const;
};

} // namespace pge
