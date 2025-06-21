
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
  core::serialize(out, m_asteroidsData.size());

  for (const auto &asteroidData : m_asteroidsData)
  {
    serializeAsteroidData(out, asteroidData);
  }

  return out;
}

bool AsteroidListMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_systemDbId);

  std::size_t count;
  ok &= core::deserialize(in, count);

  m_asteroidsData.clear();

  for (std::size_t id = 0u; id < count; ++id)
  {
    AsteroidData data;

    ok &= deserializeAsteroidData(in, data);

    m_asteroidsData.emplace_back(data);
  }

  return ok;
}

auto AsteroidListMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<AsteroidListMessage>(m_systemDbId, m_asteroidsData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
