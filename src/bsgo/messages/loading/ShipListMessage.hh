
#pragma once

#include "NetworkMessage.hh"
#include "ShipData.hh"
#include "Uuid.hh"

namespace bsgo {

class ShipListMessage : public NetworkMessage
{
  public:
  ShipListMessage();
  ShipListMessage(const Uuid systemDbId, const std::vector<ShipData> &shipsData);
  ~ShipListMessage() override = default;

  auto getSystemDbId() const -> Uuid;
  auto getShipsData() const -> const std::vector<ShipData> &;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_systemDbId{};
  std::vector<ShipData> m_shipsData{};
};

} // namespace bsgo
