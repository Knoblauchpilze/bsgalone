
#include "AsteroidListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

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

auto AsteroidListMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_systemDbId);

  core::serialize(out, m_asteroidsData);

  return out;
}

bool AsteroidListMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_systemDbId);

  ok &= core::deserialize(in, m_asteroidsData);

  return ok;
}

auto AsteroidListMessage::clone() const -> bsgalone::core::IMessagePtr
{
  auto clone = std::make_unique<AsteroidListMessage>(m_systemDbId, m_asteroidsData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
