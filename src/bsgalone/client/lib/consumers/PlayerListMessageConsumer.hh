
#pragma once

#include "AbstractGameMessageConsumer.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "PlayerData.hh"

namespace bsgalone::client {

class PlayerListMessageConsumer : public AbstractGameMessageConsumer
{
  public:
  PlayerListMessageConsumer(core::DatabaseEntityMapper &entityMapper,
                            core::CoordinatorShPtr coordinator);
  ~PlayerListMessageConsumer() override = default;

  protected:
  void onMessageReceivedInternal(const core::IMessage &message) override;

  private:
  core::DatabaseEntityMapper &m_entityMapper;
  core::CoordinatorShPtr m_coordinator{};

  void registerPlayer(const core::PlayerData &data) const;
};

} // namespace bsgalone::client
