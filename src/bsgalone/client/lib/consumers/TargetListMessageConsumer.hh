
#pragma once

#include "AbstractGameMessageConsumer.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "Target.hh"

namespace bsgalone::client {

class TargetListMessageConsumer : public AbstractGameMessageConsumer
{
  public:
  TargetListMessageConsumer(core::DatabaseEntityMapper &entityMapper,
                            core::CoordinatorShPtr coordinator);
  ~TargetListMessageConsumer() override = default;

  protected:
  void onMessageReceivedInternal(const core::IMessage &message) override;

  private:
  core::DatabaseEntityMapper &m_entityMapper;
  core::CoordinatorShPtr m_coordinator{};

  void registerTarget(const core::Target &data) const;
};

} // namespace bsgalone::client
