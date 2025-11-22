
#pragma once

#include "AbstractMessageConsumer.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "TargetData.hh"

namespace pge {

class TargetListMessageConsumer : public bsgo::AbstractMessageConsumer
{
  public:
  TargetListMessageConsumer(bsgo::DatabaseEntityMapper &entityMapper,
                            bsgo::CoordinatorShPtr coordinator);
  ~TargetListMessageConsumer() override = default;

  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  bsgo::DatabaseEntityMapper &m_entityMapper;
  bsgo::CoordinatorShPtr m_coordinator{};

  void registerTarget(const bsgo::TargetData &data) const;
};

} // namespace pge
