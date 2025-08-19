
#include "ShipListMessage.hh"
#include "DataSerialization.hh"
#include "SerializationUtils.hh"

namespace bsgo {

ShipListMessage::ShipListMessage()
  : NetworkMessage(MessageType::SHIP_LIST)
{}

ShipListMessage::ShipListMessage(const std::vector<ShipData> &shipsData)
  : NetworkMessage(MessageType::SHIP_LIST)
  , m_shipsData(shipsData)
{}

auto ShipListMessage::getShipsData() const -> const std::vector<ShipData> &
{
  return m_shipsData;
}

auto ShipListMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

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
  auto clone = std::make_unique<ShipListMessage>(m_shipsData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
