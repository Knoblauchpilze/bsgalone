
#include "EntityAddedMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

EntityAddedMessage::EntityAddedMessage()
  : NetworkMessage(bsgalone::core::MessageType::ENTITY_ADDED)
{}

EntityAddedMessage::EntityAddedMessage(const Uuid systemDbId)
  : NetworkMessage(bsgalone::core::MessageType::ENTITY_ADDED)
  , m_systemDbId(systemDbId)
{}

auto EntityAddedMessage::getSystemDbId() const -> Uuid
{
  return m_systemDbId;
}

auto EntityAddedMessage::getEntityKind() const -> bsgalone::core::EntityKind
{
  if (!m_entityKind)
  {
    error("Expected entity kind to be defined but it was not");
  }
  return *m_entityKind;
}

void EntityAddedMessage::setAsteroidData(const AsteroidData &data)
{
  m_entityKind   = bsgalone::core::EntityKind::ASTEROID;
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
  m_entityKind = bsgalone::core::EntityKind::SHIP;
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
  m_entityKind = bsgalone::core::EntityKind::OUTPOST;
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
  m_entityKind = bsgalone::core::EntityKind::PLAYER;
  m_asteroidData.reset();
  m_shipData.reset();
  m_outpostData.reset();
  m_playerData = data;
}

auto EntityAddedMessage::tryGetPlayerData() const -> std::optional<PlayerData>
{
  return m_playerData;
}

auto EntityAddedMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  if (!m_entityKind)
  {
    error("Expected entity kind to be defined but it was not");
  }

  core::serialize(out, m_systemDbId);
  core::serialize(out, *m_entityKind);

  switch (*m_entityKind)
  {
    case bsgalone::core::EntityKind::ASTEROID:
      core::serialize(out, *m_asteroidData);
      break;
    case bsgalone::core::EntityKind::SHIP:
      core::serialize(out, *m_shipData);
      break;
    case bsgalone::core::EntityKind::OUTPOST:
      core::serialize(out, *m_outpostData);
      break;
    case bsgalone::core::EntityKind::PLAYER:
      core::serialize(out, *m_playerData);
      break;
    default:
      error("Unsupported entity kind for serialization: " + str(*m_entityKind));
      break;
  }

  return out;
}

namespace {
template<typename T>
auto deserializeData(std::istream &in, std::optional<T> &out) -> bool
{
  T data{};
  bool ok = core::deserialize(in, data);

  out.reset();
  if (ok)
  {
    out = data;
  }

  return ok;
}
} // namespace

bool EntityAddedMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_systemDbId);
  // The serialization makes sure that the entity kind is always defined.
  bsgalone::core::EntityKind kind{};
  ok &= core::deserialize(in, kind);
  m_entityKind = kind;

  m_asteroidData.reset();
  m_shipData.reset();
  m_outpostData.reset();

  switch (*m_entityKind)
  {
    case bsgalone::core::EntityKind::ASTEROID:
      ok &= deserializeData(in, m_asteroidData);
      break;
    case bsgalone::core::EntityKind::SHIP:
      ok &= deserializeData(in, m_shipData);
      break;
    case bsgalone::core::EntityKind::OUTPOST:
      ok &= deserializeData(in, m_outpostData);
      break;
    case bsgalone::core::EntityKind::PLAYER:
      ok &= deserializeData(in, m_playerData);
      break;
    default:
      error("Unsupported entity kind for deserialization: " + str(*m_entityKind));
      break;
  }

  return ok;
}

auto EntityAddedMessage::clone() const -> bsgalone::core::IMessagePtr
{
  auto clone = std::make_unique<EntityAddedMessage>(m_systemDbId);
  switch (*m_entityKind)
  {
    case bsgalone::core::EntityKind::ASTEROID:
      clone->setAsteroidData(*m_asteroidData);
      break;
    case bsgalone::core::EntityKind::SHIP:
      clone->setShipData(*m_shipData);
      break;
    case bsgalone::core::EntityKind::OUTPOST:
      clone->setOutpostData(*m_outpostData);
      break;
    case bsgalone::core::EntityKind::PLAYER:
      clone->setPlayerData(*m_playerData);
      break;
    default:
      error("Unsupported entity kind for cloning: " + str(*m_entityKind));
      break;
  }

  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
