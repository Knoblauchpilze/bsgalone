
#include "PlayerListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

bool PlayerListMessage::PlayerData::operator==(const PlayerData &rhs) const
{
  return playerDbId == rhs.playerDbId && name == rhs.name;
}

PlayerListMessage::PlayerListMessage()
  : NetworkMessage(MessageType::PLAYER_LIST)
{}

PlayerListMessage::PlayerListMessage(const Uuid systemDbId,
                                     const std::vector<PlayerData> &playersData)
  : NetworkMessage(MessageType::PLAYER_LIST)
  , m_systemDbId(systemDbId)
  , m_playersData(playersData)
{}

auto PlayerListMessage::getSystemDbId() const -> Uuid
{
  return m_systemDbId;
}

auto PlayerListMessage::getPlayersData() const -> const std::vector<PlayerData> &
{
  return m_playersData;
}

auto PlayerListMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_systemDbId);
  core::serialize(out, m_playersData.size());

  for (const auto &playerData : m_playersData)
  {
    core::serialize(out, playerData.playerDbId);
    core::serialize(out, playerData.name);
  }

  return out;
}

bool PlayerListMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_systemDbId);

  std::size_t count;
  ok &= core::deserialize(in, count);

  m_playersData.clear();

  for (std::size_t id = 0u; id < count; ++id)
  {
    Uuid playerDbId;
    std::string name;

    ok &= core::deserialize(in, playerDbId);
    ok &= core::deserialize(in, name);

    m_playersData.emplace_back(playerDbId, name);
  }

  return ok;
}

auto PlayerListMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<PlayerListMessage>(m_systemDbId, m_playersData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
