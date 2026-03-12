
#pragma once

#include "Faction.hh"
#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class PlayerLoginDataMessage : public NetworkMessage
{
  public:
  PlayerLoginDataMessage();
  ~PlayerLoginDataMessage() override = default;

  auto getFaction() const -> Faction;
  auto getActiveShipDbId() const -> Uuid;
  bool isDocked() const;
  auto getSystemDbId() const -> Uuid;

  void setFaction(const Faction faction);
  void setActiveShipDbId(const Uuid activeShipDbId);
  void setDocked(const bool docked);
  void setSystemDbId(const Uuid systemDbId);

  auto clone() const -> IMessagePtr override;

  private:
  Faction m_faction{};
  Uuid m_activeShipDbId{};
  bool m_docked{};
  Uuid m_systemDbId{};

  friend auto operator<<(std::ostream &out, const PlayerLoginDataMessage &message)
    -> std::ostream &;
  friend auto operator>>(std::istream &in, PlayerLoginDataMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const PlayerLoginDataMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, PlayerLoginDataMessage &message) -> std::istream &;

} // namespace bsgalone::core
