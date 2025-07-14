
#pragma once

#include "Faction.hh"
#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgo {

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

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Faction m_faction{};
  Uuid m_activeShipDbId{};
  bool m_docked{};
  Uuid m_systemDbId{};
};

} // namespace bsgo
