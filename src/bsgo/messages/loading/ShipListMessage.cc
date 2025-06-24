
#include "ShipListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

ShipListMessage::ShipListMessage()
  : NetworkMessage(MessageType::SHIP_LIST)
{}

ShipListMessage::ShipListMessage(const Uuid systemDbId, const std::vector<ShipData> &asteroidsData)
  : NetworkMessage(MessageType::SHIP_LIST)
  , m_systemDbId(systemDbId)
  , m_shipsData(asteroidsData)
{}

auto ShipListMessage::getSystemDbId() const -> Uuid
{
  return m_systemDbId;
}

auto ShipListMessage::getShipsData() const -> const std::vector<ShipData> &
{
  return m_shipsData;
}

auto ShipListMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_systemDbId);

  core::serialize(out, m_shipsData.size());
  for (const auto &shipData : m_shipsData)
  {
    serializeShipData(out, shipData);
  }

  return out;
}

bool ShipListMessage::deserialize(std::istream &in)
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
    ShipData data;

    ok &= deserializeShipData(in, data);

    m_shipsData.emplace_back(data);
  }

  return ok;
}

auto ShipListMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<ShipListMessage>(m_systemDbId, m_shipsData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
