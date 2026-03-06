

#include "ShipSelectedMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

ShipSelectedMessage::ShipSelectedMessage()
  : AbstractPlayerMessage(MessageType::SHIP_SELECTED, Uuid{0}, Uuid{0})
{}

ShipSelectedMessage::ShipSelectedMessage(const Uuid playerDbId,
                                         const Uuid systemDbId,
                                         const Uuid shipDbId)
  : AbstractPlayerMessage(MessageType::SHIP_SELECTED, playerDbId, systemDbId)
  , m_shipDbId(shipDbId)
{}

auto ShipSelectedMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto ShipSelectedMessage::serialize(std::ostream &out) const -> std::ostream &
{
  ::core::serialize(out, m_messageType);
  ::core::serialize(out, m_playerDbId);
  ::core::serialize(out, m_systemDbId);

  ::core::serialize(out, m_shipDbId);

  return out;
}

bool ShipSelectedMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= ::core::deserialize(in, m_messageType);
  ok &= ::core::deserialize(in, m_playerDbId);
  ok &= ::core::deserialize(in, m_systemDbId);

  ok &= ::core::deserialize(in, m_shipDbId);

  return ok;
}

auto ShipSelectedMessage::clone() const -> IMessagePtr
{
  return std::make_unique<ShipSelectedMessage>(m_playerDbId, m_systemDbId, m_shipDbId);
}

auto ShipSelectedMessage::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  ShipSelectedMessage message;

  if (!message.deserialize(in))
  {
    return {};
  }

  return message.clone();
}

} // namespace bsgalone::core
