
#pragma once

#include "NetworkMessage.hh"
#include "PlayerShipData.hh"
#include "Uuid.hh"

namespace bsgo {

class PlayerShipListMessage : public NetworkMessage
{
  public:
  PlayerShipListMessage();
  PlayerShipListMessage(const std::vector<PlayerShipData> &shipsData);
  ~PlayerShipListMessage() override = default;

  auto tryGetSystemDbId() const -> std::optional<Uuid>;
  auto tryGetPlayerDbId() const -> std::optional<Uuid>;
  auto getShipsData() const -> const std::vector<PlayerShipData> &;

  void setSystemDbId(const Uuid systemDbId);
  void setPlayerDbId(const Uuid playerDbId);

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  std::optional<Uuid> m_systemDbId{};
  std::optional<Uuid> m_playerDbId{};
  std::vector<PlayerShipData> m_shipsData{};
};

} // namespace bsgo
