

#include "HangarMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

HangarMessage::HangarMessage()
  : NetworkMessage(MessageType::HANGAR)
{}

HangarMessage::HangarMessage(const Uuid shipDbId)
  : NetworkMessage(MessageType::HANGAR)
{
  m_ship.dbId = shipDbId;
}

HangarMessage::HangarMessage(const PlayerShipData ship)
  : NetworkMessage(MessageType::HANGAR)
  , m_ship(ship)
{}

auto HangarMessage::getShipDbId() const -> Uuid
{
  return m_ship.dbId;
}

auto HangarMessage::getShip() const -> PlayerShipData
{
  return m_ship;
}

auto HangarMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<HangarMessage>(m_ship);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const HangarMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_ship);

  return out;
}

auto operator>>(std::istream &in, HangarMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_ship);

  return in;
}

} // namespace bsgalone::core
