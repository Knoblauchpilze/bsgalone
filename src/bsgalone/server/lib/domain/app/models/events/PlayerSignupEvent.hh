
#pragma once

#include "ClientId.hh"
#include "Faction.hh"
#include "IGameEvent.hh"
#include "Uuid.hh"
#include <optional>

namespace bsgalone::core {

class PlayerSignupEvent : public IGameEvent
{
  public:
  PlayerSignupEvent(const net::ClientId clientId);
  ~PlayerSignupEvent() override = default;

  bool successfulSignup() const;

  auto getClientId() const -> net::ClientId;
  auto tryGetPlayerDbId() const -> std::optional<Uuid>;
  auto tryGetFaction() const -> std::optional<Faction>;

  void setPlayerDbId(const Uuid playerDbId);
  void setFaction(const Faction role);

  auto clone() const -> IGameEventPtr override;

  private:
  net::ClientId m_clientId{};
  std::optional<Uuid> m_playerDbId{};
  std::optional<Faction> m_faction{};
};

} // namespace bsgalone::core
