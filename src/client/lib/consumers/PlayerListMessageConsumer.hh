
#pragma once

#include "AbstractMessageConsumer.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "Uuid.hh"

namespace pge {

class PlayerListMessageConsumer : public bsgo::AbstractMessageConsumer
{
  public:
  PlayerListMessageConsumer(bsgo::DatabaseEntityMapper &entityMapper,
                            bsgo::CoordinatorShPtr coordinator);
  ~PlayerListMessageConsumer() override = default;

  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  bsgo::DatabaseEntityMapper &m_entityMapper;
  bsgo::CoordinatorShPtr m_coordinator{};

  void registerPlayer(const bsgo::Uuid playerDbId, const std::string &name) const;
};

} // namespace pge
