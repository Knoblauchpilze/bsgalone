
#pragma once

#include "ClientId.hh"
#include "GameRole.hh"
#include "IGameEvent.hh"
#include "Uuid.hh"
#include <optional>

namespace bsgalone::core {

class PlayerLoginEvent : public IGameEvent
{
  public:
  PlayerLoginEvent(const net::ClientId clientId);
  ~PlayerLoginEvent() override = default;

  bool successfulLogin() const;

  auto getClientId() const -> net::ClientId;
  auto tryGetPlayerDbId() const -> std::optional<Uuid>;
  auto tryGetRole() const -> std::optional<GameRole>;
  auto tryGetSystemDbId() const -> std::optional<Uuid>;

  void setPlayerDbId(const Uuid playerDbId);
  void setRole(const GameRole role);
  void setSystemDbId(const Uuid systemDbId);

  auto clone() const -> IGameEventPtr override;

  private:
  net::ClientId m_clientId{};
  std::optional<Uuid> m_playerDbId{};
  std::optional<GameRole> m_role{};
  std::optional<Uuid> m_systemDbId{};
};

} // namespace bsgalone::core
