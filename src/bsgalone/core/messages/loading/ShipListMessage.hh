
#pragma once

#include "Faction.hh"
#include "NetworkMessage.hh"
#include "ShipData.hh"
#include "Uuid.hh"

namespace bsgo {

class ShipListMessage : public bsgalone::core::NetworkMessage
{
  public:
  ShipListMessage();
  ShipListMessage(const Faction faction, const std::vector<ShipData> &shipsData);
  ~ShipListMessage() override = default;

  auto getFaction() const -> Faction;
  auto getShipsData() const -> const std::vector<ShipData> &;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> bsgalone::core::IMessagePtr override;

  private:
  Faction m_faction{};
  std::vector<ShipData> m_shipsData{};
};

} // namespace bsgo
