
#pragma once

#include "NetworkMessage.hh"
#include "PlayerData.hh"
#include "Uuid.hh"

namespace bsgo {

class PlayerListMessage : public NetworkMessage
{
  public:
  PlayerListMessage();
  PlayerListMessage(const Uuid systemDbId, const std::vector<PlayerData> &playersData);
  ~PlayerListMessage() override = default;

  auto getSystemDbId() const -> Uuid;
  auto getPlayersData() const -> const std::vector<PlayerData> &;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_systemDbId{};
  std::vector<PlayerData> m_playersData{};
};

} // namespace bsgo
