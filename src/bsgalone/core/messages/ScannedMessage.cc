
#include "ScannedMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

ScannedMessage::ScannedMessage()
  : NetworkMessage(MessageType::SCANNED)
{}

ScannedMessage::ScannedMessage(const Uuid playerDbId, const Uuid asteroidDbId)
  : NetworkMessage(MessageType::SCANNED)
  , m_playerDbId(playerDbId)
  , m_asteroidDbId(asteroidDbId)
{}

auto ScannedMessage::getPlayerDbId() const -> Uuid
{
  return m_playerDbId;
}

auto ScannedMessage::getAsteroidDbId() const -> Uuid
{
  return m_asteroidDbId;
}

auto ScannedMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<ScannedMessage>(m_playerDbId, m_asteroidDbId);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const ScannedMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_playerDbId);
  ::core::serialize(out, message.m_asteroidDbId);

  return out;
}

auto operator>>(std::istream &in, ScannedMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_playerDbId);
  ::core::deserialize(in, message.m_asteroidDbId);

  return in;
}

} // namespace bsgalone::core
