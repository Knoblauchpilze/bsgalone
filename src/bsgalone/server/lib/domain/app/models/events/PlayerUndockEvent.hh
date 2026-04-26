
#pragma once

#include "IGameEvent.hh"
#include "Uuid.hh"

namespace bsgalone::server {

class PlayerUndockEvent : public IGameEvent
{
  public:
  explicit PlayerUndockEvent(const core::Uuid playerDbId);
  ~PlayerUndockEvent() override = default;

  auto getPlayerDbId() const -> core::Uuid;

  auto clone() const -> IGameEventPtr override;

  private:
  core::Uuid m_playerDbId{};
};

} // namespace bsgalone::server
