
#pragma once

#include "NetworkMessage.hh"
#include "PlayerData.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class PlayerListMessage : public NetworkMessage
{
  public:
  PlayerListMessage();
  PlayerListMessage(const Uuid systemDbId, const std::vector<PlayerData> &playersData);
  ~PlayerListMessage() override = default;

  auto getSystemDbId() const -> Uuid;
  auto getPlayersData() const -> const std::vector<PlayerData> &;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_systemDbId{};
  std::vector<PlayerData> m_playersData{};

  friend auto operator<<(std::ostream &out, const PlayerListMessage &message) -> std::ostream &;
  friend auto operator>>(std::istream &in, PlayerListMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const PlayerListMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, PlayerListMessage &message) -> std::istream &;

} // namespace bsgalone::core
