
#pragma once

#include "AbstractMessageConsumer.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "ScannedMessage.hh"

namespace pge {

class SystemMessageConsumer : public bsgo::AbstractMessageConsumer
{
  public:
  SystemMessageConsumer(const bsgo::DatabaseEntityMapper &entityMapper,
                        bsgo::CoordinatorShPtr coordinator);
  ~SystemMessageConsumer() override = default;

  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  const bsgo::DatabaseEntityMapper &m_entityMapper;
  bsgo::CoordinatorShPtr m_coordinator{};

  void handleScanOperation(const bsgo::ScannedMessage &message) const;
};

} // namespace pge
