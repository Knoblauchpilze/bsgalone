
#include "PlayerListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

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

auto PlayerListMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<PlayerListMessage>(m_systemDbId, m_playersData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const PlayerListMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_systemDbId);
  ::core::serialize(out, message.m_playersData);

  return out;
}

auto operator>>(std::istream &in, PlayerListMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_systemDbId);
  ::core::deserialize(in, message.m_playersData);

  return in;
}

} // namespace bsgalone::core
