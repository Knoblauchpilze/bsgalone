
#pragma once

#include "ClientId.hh"
#include "Faction.hh"
#include "GameRole.hh"
#include "IGameEvent.hh"
#include "Uuid.hh"
#include <optional>

namespace bsgalone::server {

class PlayerLoginEvent : public IGameEvent
{
  public:
  explicit PlayerLoginEvent(const net::ClientId clientId);
  ~PlayerLoginEvent() override = default;

  bool successfulLogin() const;

  auto getClientId() const -> net::ClientId;
  auto tryGetPlayerDbId() const -> std::optional<core::Uuid>;
  auto tryGetFaction() const -> std::optional<core::Faction>;
  auto tryGetRole() const -> std::optional<core::GameRole>;

  void setPlayerDbId(const core::Uuid playerDbId);
  void setFaction(const core::Faction faction);
  void setRole(const core::GameRole role);

  auto clone() const -> IGameEventPtr override;

  private:
  net::ClientId m_clientId{};
  std::optional<core::Uuid> m_playerDbId{};
  std::optional<core::Faction> m_faction{};
  std::optional<core::GameRole> m_role{};
};

} // namespace bsgalone::server
