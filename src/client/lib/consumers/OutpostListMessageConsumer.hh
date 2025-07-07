
#pragma once

#include "AbstractMessageConsumer.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "OutpostData.hh"
#include "Uuid.hh"

namespace pge {

class OutpostListMessageConsumer : public bsgo::AbstractMessageConsumer
{
  public:
  OutpostListMessageConsumer(bsgo::DatabaseEntityMapper &entityMapper,
                             bsgo::CoordinatorShPtr coordinator);
  ~OutpostListMessageConsumer() override = default;

  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  bsgo::DatabaseEntityMapper &m_entityMapper;
  bsgo::CoordinatorShPtr m_coordinator{};

  void registerOutpost(const bsgo::OutpostData &data) const;
};

} // namespace pge
