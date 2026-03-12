
#include "ShipListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

ShipListMessage::ShipListMessage()
  : NetworkMessage(MessageType::SHIP_LIST)
{}

ShipListMessage::ShipListMessage(const Faction faction, const std::vector<ShipData> &shipsData)
  : NetworkMessage(MessageType::SHIP_LIST)
  , m_faction(faction)
  , m_shipsData(shipsData)
{}

auto ShipListMessage::getFaction() const -> Faction
{
  return m_faction;
}

auto ShipListMessage::getShipsData() const -> const std::vector<ShipData> &
{
  return m_shipsData;
}

auto ShipListMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<ShipListMessage>(m_faction, m_shipsData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const ShipListMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_faction);
  ::core::serialize(out, message.m_shipsData);

  return out;
}

auto operator>>(std::istream &in, ShipListMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_faction);
  ::core::deserialize(in, message.m_shipsData);

  return in;
}

} // namespace bsgalone::core
