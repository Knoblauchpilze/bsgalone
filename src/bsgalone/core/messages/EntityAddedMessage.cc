
#include "EntityAddedMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

EntityAddedMessage::EntityAddedMessage()
  : NetworkMessage(MessageType::ENTITY_ADDED)
{}

EntityAddedMessage::EntityAddedMessage(const Uuid systemDbId)
  : NetworkMessage(MessageType::ENTITY_ADDED)
  , m_systemDbId(systemDbId)
{}

auto EntityAddedMessage::getSystemDbId() const -> Uuid
{
  return m_systemDbId;
}

auto EntityAddedMessage::getEntityKind() const -> EntityKind
{
  if (!m_entityKind)
  {
    throw std::runtime_error("Expected entity kind to be defined but it was not");
  }
  return *m_entityKind;
}

void EntityAddedMessage::setAsteroidData(const AsteroidData &data)
{
  m_entityKind   = EntityKind::ASTEROID;
  m_asteroidData = data;
  m_shipData.reset();
  m_outpostData.reset();
  m_playerData.reset();
}

auto EntityAddedMessage::tryGetAsteroidData() const -> std::optional<AsteroidData>
{
  return m_asteroidData;
}

void EntityAddedMessage::setShipData(const PlayerShipData &data)
{
  m_entityKind = EntityKind::SHIP;
  m_asteroidData.reset();
  m_shipData = data;
  m_outpostData.reset();
  m_playerData.reset();
}

auto EntityAddedMessage::tryGetShipData() const -> std::optional<PlayerShipData>
{
  return m_shipData;
}

void EntityAddedMessage::setOutpostData(const OutpostData &data)
{
  m_entityKind = EntityKind::OUTPOST;
  m_asteroidData.reset();
  m_shipData.reset();
  m_outpostData = data;
  m_playerData.reset();
}

auto EntityAddedMessage::tryGetOutpostData() const -> std::optional<OutpostData>
{
  return m_outpostData;
}

void EntityAddedMessage::setPlayerData(const PlayerData &data)
{
  m_entityKind = EntityKind::PLAYER;
  m_asteroidData.reset();
  m_shipData.reset();
  m_outpostData.reset();
  m_playerData = data;
}

auto EntityAddedMessage::tryGetPlayerData() const -> std::optional<PlayerData>
{
  return m_playerData;
}

auto EntityAddedMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<EntityAddedMessage>(m_systemDbId);
  switch (*m_entityKind)
  {
    case EntityKind::ASTEROID:
      clone->setAsteroidData(*m_asteroidData);
      break;
    case EntityKind::SHIP:
      clone->setShipData(*m_shipData);
      break;
    case EntityKind::OUTPOST:
      clone->setOutpostData(*m_outpostData);
      break;
    case EntityKind::PLAYER:
      clone->setPlayerData(*m_playerData);
      break;
    default:
      throw std::invalid_argument("Unsupported entity kind for cloning: " + str(*m_entityKind));
      break;
  }

  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const EntityAddedMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);

  if (!message.m_entityKind)
  {
    throw std::runtime_error("Expected entity kind to be defined but it was not");
  }

  ::core::serialize(out, message.m_systemDbId);
  ::core::serialize(out, *message.m_entityKind);

  switch (*message.m_entityKind)
  {
    case EntityKind::ASTEROID:
      ::core::serialize(out, *message.m_asteroidData);
      break;
    case EntityKind::SHIP:
      ::core::serialize(out, *message.m_shipData);
      break;
    case EntityKind::OUTPOST:
      ::core::serialize(out, *message.m_outpostData);
      break;
    case EntityKind::PLAYER:
      ::core::serialize(out, *message.m_playerData);
      break;
    default:
      throw std::invalid_argument("Unsupported entity kind for serialization: "
                                  + str(*message.m_entityKind));
      break;
  }

  return out;
}

namespace {
template<typename T>
void deserializeData(std::istream &in, std::optional<T> &out)
{
  T data{};
  bool ok = ::core::deserialize(in, data);

  out.reset();
  if (ok)
  {
    out = data;
  }
}
} // namespace

auto operator>>(std::istream &in, EntityAddedMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);

  ::core::deserialize(in, message.m_systemDbId);
  // The serialization makes sure that the entity kind is always defined.
  EntityKind kind{};
  ::core::deserialize(in, kind);
  message.m_entityKind = kind;

  message.m_asteroidData.reset();
  message.m_shipData.reset();
  message.m_outpostData.reset();

  switch (*message.m_entityKind)
  {
    case EntityKind::ASTEROID:
      deserializeData(in, message.m_asteroidData);
      break;
    case EntityKind::SHIP:
      deserializeData(in, message.m_shipData);
      break;
    case EntityKind::OUTPOST:
      deserializeData(in, message.m_outpostData);
      break;
    case EntityKind::PLAYER:
      deserializeData(in, message.m_playerData);
      break;
    default:
      throw std::invalid_argument("Unsupported entity kind for deserialization: "
                                  + str(*message.m_entityKind));
      break;
  }

  return in;
}

} // namespace bsgalone::core
