
#include "EntityRemovedMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

EntityRemovedMessage::EntityRemovedMessage()
  : NetworkMessage(MessageType::ENTITY_REMOVED)
{}

EntityRemovedMessage::EntityRemovedMessage(const Uuid entityDbId,
                                           const EntityKind entityKind,
                                           const bool dead)
  : NetworkMessage(MessageType::ENTITY_REMOVED)
  , m_entityDbId(entityDbId)
  , m_entityKind(entityKind)
  , m_dead(dead)
{}

EntityRemovedMessage::EntityRemovedMessage(const Uuid entityDbId,
                                           const EntityKind entityKind,
                                           const bool dead,
                                           const Uuid systemDbId)
  : NetworkMessage(MessageType::ENTITY_REMOVED)
  , m_entityDbId(entityDbId)
  , m_entityKind(entityKind)
  , m_dead(dead)
  , m_systemDbId(systemDbId)
{}

auto EntityRemovedMessage::getEntityDbId() const -> Uuid
{
  return m_entityDbId;
}

auto EntityRemovedMessage::getEntityKind() const -> EntityKind
{
  return m_entityKind;
}

bool EntityRemovedMessage::isDead() const
{
  return m_dead;
}

auto EntityRemovedMessage::getSystemDbId() const -> Uuid
{
  if (!m_systemDbId)
  {
    throw std::runtime_error("Expected system db id to be defined but it was not");
  }
  return *m_systemDbId;
}

auto EntityRemovedMessage::tryGetSystemDbId() const -> std::optional<Uuid>
{
  return m_systemDbId;
}

auto EntityRemovedMessage::clone() const -> IMessagePtr
{
  auto clone          = std::make_unique<EntityRemovedMessage>(m_entityDbId, m_entityKind, m_dead);
  clone->m_systemDbId = m_systemDbId;
  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const EntityRemovedMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_entityDbId);
  ::core::serialize(out, message.m_entityKind);
  ::core::serialize(out, message.m_dead);
  ::core::serialize(out, message.m_systemDbId);

  return out;
}

auto operator>>(std::istream &in, EntityRemovedMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_entityDbId);
  ::core::deserialize(in, message.m_entityKind);
  ::core::deserialize(in, message.m_dead);
  ::core::deserialize(in, message.m_systemDbId);

  return in;
}

} // namespace bsgalone::core
