
#include "ShipListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

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

auto ShipListMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_faction);

  core::serialize(out, m_shipsData);

  return out;
}

bool ShipListMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_faction);

  ok &= core::deserialize(in, m_shipsData);

  return ok;
}

auto ShipListMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<ShipListMessage>(m_faction, m_shipsData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
