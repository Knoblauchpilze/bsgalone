
#pragma once

#include "NetworkMessage.hh"
#include "PlayerShipData.hh"
#include "Uuid.hh"

namespace bsgalone::core {

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

  auto clone() const -> IMessagePtr override;

  private:
  std::optional<Uuid> m_systemDbId{};
  std::optional<Uuid> m_playerDbId{};
  std::vector<PlayerShipData> m_shipsData{};

  friend auto operator<<(std::ostream &out, const PlayerShipListMessage &message) -> std::ostream &;
  friend auto operator>>(std::istream &in, PlayerShipListMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const PlayerShipListMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, PlayerShipListMessage &message) -> std::istream &;

} // namespace bsgalone::core
