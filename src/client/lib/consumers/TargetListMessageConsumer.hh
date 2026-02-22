
#pragma once

#include "AbstractGameMessageConsumer.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "Target.hh"

namespace pge {

class TargetListMessageConsumer : public AbstractGameMessageConsumer
{
  public:
  TargetListMessageConsumer(bsgo::DatabaseEntityMapper &entityMapper,
                            bsgo::CoordinatorShPtr coordinator);
  ~TargetListMessageConsumer() override = default;

  protected:
  void onMessageReceivedInternal(const bsgalone::core::IMessage &message) override;

  private:
  bsgo::DatabaseEntityMapper &m_entityMapper;
  bsgo::CoordinatorShPtr m_coordinator{};

  void registerTarget(const bsgalone::core::Target &data) const;
};

} // namespace pge
