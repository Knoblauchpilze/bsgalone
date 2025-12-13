
#pragma once

#include "AbstractGameMessageConsumer.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "PlayerData.hh"

namespace pge {

class PlayerListMessageConsumer : public AbstractGameMessageConsumer
{
  public:
  PlayerListMessageConsumer(bsgo::DatabaseEntityMapper &entityMapper,
                            bsgo::CoordinatorShPtr coordinator);
  ~PlayerListMessageConsumer() override = default;

  protected:
  void onMessageReceivedInternal(const bsgo::IMessage &message) override;

  private:
  bsgo::DatabaseEntityMapper &m_entityMapper;
  bsgo::CoordinatorShPtr m_coordinator{};

  void registerPlayer(const bsgo::PlayerData &data) const;
};

} // namespace pge
