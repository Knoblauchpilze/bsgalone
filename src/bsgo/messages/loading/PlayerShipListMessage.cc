
#include "PlayerShipListMessage.hh"
#include "DataSerialization.hh"
#include "SerializationUtils.hh"

namespace bsgo {

PlayerShipListMessage::PlayerShipListMessage()
  : NetworkMessage(MessageType::PLAYER_SHIP_LIST)
{}

PlayerShipListMessage::PlayerShipListMessage(const Uuid systemDbId,
                                             const std::vector<PlayerShipData> &asteroidsData)
  : NetworkMessage(MessageType::PLAYER_SHIP_LIST)
  , m_systemDbId(systemDbId)
  , m_shipsData(asteroidsData)
{}

auto PlayerShipListMessage::getSystemDbId() const -> Uuid
{
  return m_systemDbId;
}

auto PlayerShipListMessage::getShipsData() const -> const std::vector<PlayerShipData> &
{
  return m_shipsData;
}

auto PlayerShipListMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_systemDbId);

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
  auto clone = std::make_unique<PlayerShipListMessage>(m_systemDbId, m_shipsData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
