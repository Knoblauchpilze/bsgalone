
#pragma once

#include "Faction.hh"
#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class PlayerLoginDataMessage : public NetworkMessage
{
  public:
  PlayerLoginDataMessage();
  PlayerLoginDataMessage(const Faction faction, const Uuid activeShipDbId, const bool docked);
  ~PlayerLoginDataMessage() override = default;

  auto getFaction() const -> Faction;
  auto getActiveShipDbId() const -> Uuid;
  bool isDocked() const;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Faction m_faction{};
  Uuid m_activeShipDbId{};
  bool m_docked{};
};

} // namespace bsgo
