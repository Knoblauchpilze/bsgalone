
#pragma once

#include "AbstractMessageConsumer.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "EntityDiedMessage.hh"
#include "ScannedMessage.hh"

namespace pge {

class SystemMessageConsumer : public bsgo::AbstractMessageConsumer
{
  public:
  SystemMessageConsumer(bsgo::DatabaseEntityMapper &entityMapper,
                        bsgo::CoordinatorShPtr coordinator);
  ~SystemMessageConsumer() override = default;

  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  bsgo::DatabaseEntityMapper &m_entityMapper;
  bsgo::CoordinatorShPtr m_coordinator{};

  void handleScanOperation(const bsgo::ScannedMessage &message) const;
  void handleEntityDeath(const bsgo::EntityDiedMessage &message) const;
};

} // namespace pge
