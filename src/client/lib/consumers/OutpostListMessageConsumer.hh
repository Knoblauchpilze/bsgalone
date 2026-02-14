
#pragma once

#include "AbstractGameMessageConsumer.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "OutpostData.hh"

namespace pge {

class OutpostListMessageConsumer : public AbstractGameMessageConsumer
{
  public:
  OutpostListMessageConsumer(bsgo::DatabaseEntityMapper &entityMapper,
                             bsgo::CoordinatorShPtr coordinator);
  ~OutpostListMessageConsumer() override = default;

  protected:
  void onMessageReceivedInternal(const bsgalone::core::IMessage &message) override;

  private:
  bsgo::DatabaseEntityMapper &m_entityMapper;
  bsgo::CoordinatorShPtr m_coordinator{};

  void registerOutpost(const bsgo::OutpostData &data) const;
};

} // namespace pge
