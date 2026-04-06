
#pragma once

#include "ClientId.hh"
#include "Faction.hh"
#include "IGameEvent.hh"
#include "Uuid.hh"
#include <optional>

namespace bsgalone::server {

class PlayerSignupEvent : public IGameEvent
{
  public:
  PlayerSignupEvent(const net::ClientId clientId);
  ~PlayerSignupEvent() override = default;

  bool successfulSignup() const;

  auto getClientId() const -> net::ClientId;
  auto tryGetPlayerDbId() const -> std::optional<core::Uuid>;
  auto tryGetFaction() const -> std::optional<core::Faction>;

  void setPlayerDbId(const core::Uuid playerDbId);
  void setFaction(const core::Faction role);

  auto clone() const -> IGameEventPtr override;

  private:
  net::ClientId m_clientId{};
  std::optional<core::Uuid> m_playerDbId{};
  std::optional<core::Faction> m_faction{};
};

} // namespace bsgalone::server
