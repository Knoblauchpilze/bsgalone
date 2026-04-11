
#pragma once

#include "IGameEvent.hh"
#include "Uuid.hh"

namespace bsgalone::server {

class PlayerLogoutEvent : public IGameEvent
{
  public:
  explicit PlayerLogoutEvent(const core::Uuid playerDbId);
  ~PlayerLogoutEvent() override = default;

  auto getPlayerDbId() const -> core::Uuid;

  auto clone() const -> IGameEventPtr override;

  private:
  core::Uuid m_playerDbId{};
};

} // namespace bsgalone::server
