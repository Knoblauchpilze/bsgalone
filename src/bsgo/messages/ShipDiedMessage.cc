
#include "ShipDiedMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

ShipDiedMessage::ShipDiedMessage()
  : ValidatableMessage(MessageType::SHIP_DIED)
{}

ShipDiedMessage::ShipDiedMessage(const Uuid playerDbId, Uuid shipDbId)
  : ValidatableMessage(MessageType::SHIP_DIED)
  , m_playerDbId(playerDbId)
  , m_shipDbId(shipDbId)
{}

auto ShipDiedMessage::getPlayerDbId() const -> Uuid
{
  return m_playerDbId;
}

auto ShipDiedMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto ShipDiedMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);
  utils::serialize(out, m_validated);

  utils::serialize(out, m_playerDbId);
  utils::serialize(out, m_shipDbId);

  return out;
}

bool ShipDiedMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);
  ok &= utils::deserialize(in, m_validated);

  ok &= utils::deserialize(in, m_playerDbId);
  ok &= utils::deserialize(in, m_shipDbId);

  return ok;
}

auto ShipDiedMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<ShipDiedMessage>(m_playerDbId, m_shipDbId);
  clone->copyClientIdIfDefined(*this);
  clone->validate(validated());

  return clone;
}

} // namespace bsgo
