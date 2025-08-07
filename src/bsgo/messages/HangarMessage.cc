

#include "HangarMessage.hh"
#include "DataSerialization.hh"
#include "SerializationUtils.hh"

namespace bsgo {

HangarMessage::HangarMessage()
  : ValidatableMessage(MessageType::HANGAR)
{}

HangarMessage::HangarMessage(const Uuid shipDbId)
  : ValidatableMessage(MessageType::HANGAR)
{
  m_ship.dbId = shipDbId;
}

HangarMessage::HangarMessage(const PlayerShipData ship)
  : ValidatableMessage(MessageType::HANGAR)
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

auto HangarMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);
  core::serialize(out, m_validated);

  serializePlayerShipData(out, m_ship);

  return out;
}

bool HangarMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);
  ok &= core::deserialize(in, m_validated);

  ok &= deserializePlayerShipData(in, m_ship);

  return ok;
}

auto HangarMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<HangarMessage>(m_ship);
  clone->copyClientIdIfDefined(*this);
  clone->validate(validated());

  return clone;
}

} // namespace bsgo
