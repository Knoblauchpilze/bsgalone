
#include "AsteroidListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

AsteroidListMessage::AsteroidListMessage()
  : NetworkMessage(MessageType::ASTEROID_LIST)
{}

AsteroidListMessage::AsteroidListMessage(const Uuid systemDbId,
                                         const std::vector<AsteroidData> &asteroidsData)
  : NetworkMessage(MessageType::ASTEROID_LIST)
  , m_systemDbId(systemDbId)
  , m_asteroidsData(asteroidsData)
{}

auto AsteroidListMessage::getSystemDbId() const -> Uuid
{
  return m_systemDbId;
}

auto AsteroidListMessage::getAsteroidsData() const -> const std::vector<AsteroidData> &
{
  return m_asteroidsData;
}

auto AsteroidListMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<AsteroidListMessage>(m_systemDbId, m_asteroidsData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const AsteroidListMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_systemDbId);
  ::core::serialize(out, message.m_asteroidsData);

  return out;
}

auto operator>>(std::istream &in, AsteroidListMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_systemDbId);
  ::core::deserialize(in, message.m_asteroidsData);

  return in;
}

} // namespace bsgalone::core
