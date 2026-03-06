
#pragma once

#include "AbstractMessageConsumer.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "VelocityMessage.hh"

namespace bsgalone::client {

class InternalMessageConsumer : public core::AbstractMessageConsumer
{
  public:
  InternalMessageConsumer(const core::DatabaseEntityMapper &entityMapper,
                          core::CoordinatorShPtr coordinator);
  ~InternalMessageConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  const core::DatabaseEntityMapper &m_entityMapper;
  core::CoordinatorShPtr m_coordinator{};

  void handleVelocityChanged(const core::VelocityMessage &message) const;
};

} // namespace bsgalone::client
