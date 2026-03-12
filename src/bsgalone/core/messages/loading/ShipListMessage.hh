
#pragma once

#include "Faction.hh"
#include "NetworkMessage.hh"
#include "ShipData.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class ShipListMessage : public NetworkMessage
{
  public:
  ShipListMessage();
  ShipListMessage(const Faction faction, const std::vector<ShipData> &shipsData);
  ~ShipListMessage() override = default;

  auto getFaction() const -> Faction;
  auto getShipsData() const -> const std::vector<ShipData> &;

  auto clone() const -> IMessagePtr override;

  private:
  Faction m_faction{};
  std::vector<ShipData> m_shipsData{};

  friend auto operator<<(std::ostream &out, const ShipListMessage &message) -> std::ostream &;
  friend auto operator>>(std::istream &in, ShipListMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const ShipListMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, ShipListMessage &message) -> std::istream &;

} // namespace bsgalone::core
