
#pragma once

#include "NetworkMessage.hh"
#include "PlayerShipData.hh"
#include "Uuid.hh"

namespace bsgo {

class ShipListMessage : public NetworkMessage
{
  public:
  ShipListMessage();
  ShipListMessage(const Uuid systemDbId, const std::vector<PlayerShipData> &shipsData);
  ~ShipListMessage() override = default;

  auto getSystemDbId() const -> Uuid;
  auto getShipsData() const -> const std::vector<PlayerShipData> &;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_systemDbId{};
  std::vector<PlayerShipData> m_shipsData{};
};

} // namespace bsgo
