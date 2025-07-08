
#include "EntityAddedMessage.hh"
#include "DataSerialization.hh"
#include "SerializationUtils.hh"

namespace bsgo {

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
    error("Expected entity kind to be defined but it was not");
  }
  return *m_entityKind;
}

void EntityAddedMessage::setAsteroidData(const AsteroidData &data)
{
  m_entityKind   = EntityKind::ASTEROID;
  m_asteroidData = data;
  m_shipData.reset();
  m_outpostData.reset();
}

auto EntityAddedMessage::tryGetAsteroidData() const -> std::optional<AsteroidData>
{
  return m_asteroidData;
}

void EntityAddedMessage::setShipData(const ShipData &data)
{
  m_entityKind = EntityKind::SHIP;
  m_asteroidData.reset();
  m_shipData = data;
  m_outpostData.reset();
}

auto EntityAddedMessage::tryGetShipData() const -> std::optional<ShipData>
{
  return m_shipData;
}

void EntityAddedMessage::setOutpostData(const OutpostData &data)
{
  m_entityKind = EntityKind::OUTPOST;
  m_asteroidData.reset();
  m_shipData.reset();
  m_outpostData = data;
}

auto EntityAddedMessage::tryGetOutpostData() const -> std::optional<OutpostData>
{
  return m_outpostData;
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
    case EntityKind::ASTEROID:
      serializeAsteroidData(out, *m_asteroidData);
      break;
    case EntityKind::SHIP:
      serializeShipData(out, *m_shipData);
      break;
    case EntityKind::OUTPOST:
      serializeOutpostData(out, *m_outpostData);
      break;
    default:
      error("Unsupported entity kind for serialization: " + str(*m_entityKind));
      break;
  }

  return out;
}

namespace {
auto deserializeAsteroidData(std::istream &in, std::optional<AsteroidData> &asteroid) -> bool
{
  AsteroidData data{};
  bool ok = deserializeAsteroidData(in, data);

  asteroid.reset();
  if (ok)
  {
    asteroid = data;
  }

  return ok;
}

auto deserializeShipData(std::istream &in, std::optional<ShipData> &ship) -> bool
{
  ShipData data{};
  bool ok = deserializeShipData(in, data);

  ship.reset();
  if (ok)
  {
    ship = data;
  }

  return ok;
}

auto deserializeOutpostData(std::istream &in, std::optional<OutpostData> &outpost) -> bool
{
  OutpostData data{};
  bool ok = deserializeOutpostData(in, data);

  outpost.reset();
  if (ok)
  {
    outpost = data;
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
  EntityKind kind{};
  ok &= core::deserialize(in, kind);
  m_entityKind = kind;

  m_asteroidData.reset();
  m_shipData.reset();
  m_outpostData.reset();

  switch (*m_entityKind)
  {
    case EntityKind::ASTEROID:
      ok &= deserializeAsteroidData(in, m_asteroidData);
      break;
    case EntityKind::SHIP:
      ok &= deserializeShipData(in, m_shipData);
      break;
    case EntityKind::OUTPOST:
      ok &= deserializeOutpostData(in, m_outpostData);
      break;
    default:
      error("Unsupported entity kind for deserialization: " + str(*m_entityKind));
      break;
  }

  return ok;
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
    default:
      error("Unsupported entity kind for cloning: " + str(*m_entityKind));
      break;
  }

  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
