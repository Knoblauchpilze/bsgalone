
#pragma once

#include "AbstractMessageConsumer.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "JumpRequestedMessage.hh"

namespace pge {

class JumpMessageConsumer : public bsgo::AbstractMessageConsumer
{
  public:
  JumpMessageConsumer(const bsgo::DatabaseEntityMapper &entityMapper,
                      bsgo::CoordinatorShPtr coordinator);
  ~JumpMessageConsumer() override = default;

  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  const bsgo::DatabaseEntityMapper &m_entityMapper;
  bsgo::CoordinatorShPtr m_coordinator{};

  void handleJumpRequested(const bsgo::JumpRequestedMessage &message) const;
};

} // namespace pge
