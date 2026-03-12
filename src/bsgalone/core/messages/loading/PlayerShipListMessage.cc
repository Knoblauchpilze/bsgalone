
#include "PlayerShipListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

PlayerShipListMessage::PlayerShipListMessage()
  : NetworkMessage(MessageType::PLAYER_SHIP_LIST)
{}

PlayerShipListMessage::PlayerShipListMessage(const std::vector<PlayerShipData> &shipsData)
  : NetworkMessage(MessageType::PLAYER_SHIP_LIST)
  , m_shipsData(shipsData)
{}

auto PlayerShipListMessage::tryGetSystemDbId() const -> std::optional<Uuid>
{
  return m_systemDbId;
}

auto PlayerShipListMessage::tryGetPlayerDbId() const -> std::optional<Uuid>
{
  return m_playerDbId;
}

auto PlayerShipListMessage::getShipsData() const -> const std::vector<PlayerShipData> &
{
  return m_shipsData;
}

void PlayerShipListMessage::setSystemDbId(const Uuid systemDbId)
{
  m_systemDbId = systemDbId;
}

void PlayerShipListMessage::setPlayerDbId(const Uuid playerDbId)
{
  m_playerDbId = playerDbId;
}

auto PlayerShipListMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<PlayerShipListMessage>(m_shipsData);
  if (m_systemDbId)
  {
    clone->setSystemDbId(*m_systemDbId);
  }
  if (m_playerDbId)
  {
    clone->setPlayerDbId(*m_playerDbId);
  }

  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const PlayerShipListMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_systemDbId);
  ::core::serialize(out, message.m_playerDbId);
  ::core::serialize(out, message.m_shipsData);

  return out;
}

auto operator>>(std::istream &in, PlayerShipListMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_systemDbId);
  ::core::deserialize(in, message.m_playerDbId);
  ::core::deserialize(in, message.m_shipsData);

  return in;
}

} // namespace bsgalone::core
