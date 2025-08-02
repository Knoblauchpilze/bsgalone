
#include "PlayerShipListMessage.hh"
#include "DataSerialization.hh"
#include "SerializationUtils.hh"

namespace bsgo {

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

auto PlayerShipListMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_systemDbId);
  core::serialize(out, m_playerDbId);

  core::serialize(out, m_shipsData.size());
  for (const auto &shipData : m_shipsData)
  {
    serializePlayerShipData(out, shipData);
  }

  return out;
}

bool PlayerShipListMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_systemDbId);
  ok &= core::deserialize(in, m_playerDbId);

  std::size_t count;
  ok &= core::deserialize(in, count);

  m_shipsData.clear();

  for (std::size_t id = 0u; id < count; ++id)
  {
    PlayerShipData data;

    ok &= deserializePlayerShipData(in, data);

    m_shipsData.emplace_back(data);
  }

  return ok;
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

} // namespace bsgo
