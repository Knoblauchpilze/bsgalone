
#pragma once

#include "AbstractMessageConsumer.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "VelocityMessage.hh"

namespace pge {

class InternalMessageConsumer : public bsgalone::core::AbstractMessageConsumer
{
  public:
  InternalMessageConsumer(const bsgo::DatabaseEntityMapper &entityMapper,
                          bsgo::CoordinatorShPtr coordinator);
  ~InternalMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  const bsgo::DatabaseEntityMapper &m_entityMapper;
  bsgo::CoordinatorShPtr m_coordinator{};

  void handleVelocityChanged(const bsgalone::core::VelocityMessage &message) const;
};

} // namespace pge
