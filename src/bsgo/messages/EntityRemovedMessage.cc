
#include "EntityRemovedMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

EntityRemovedMessage::EntityRemovedMessage()
  : NetworkMessage(bsgalone::core::MessageType::ENTITY_REMOVED)
{}

EntityRemovedMessage::EntityRemovedMessage(const Uuid entityDbId,
                                           const EntityKind entityKind,
                                           const bool dead)
  : NetworkMessage(bsgalone::core::MessageType::ENTITY_REMOVED)
  , m_entityDbId(entityDbId)
  , m_entityKind(entityKind)
  , m_dead(dead)
{}

EntityRemovedMessage::EntityRemovedMessage(const Uuid entityDbId,
                                           const EntityKind entityKind,
                                           const bool dead,
                                           const Uuid systemDbId)
  : NetworkMessage(bsgalone::core::MessageType::ENTITY_REMOVED)
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
    error("Expected system db id to be defined but it was not");
  }
  return *m_systemDbId;
}

auto EntityRemovedMessage::tryGetSystemDbId() const -> std::optional<Uuid>
{
  return m_systemDbId;
}

auto EntityRemovedMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_entityDbId);
  core::serialize(out, m_entityKind);
  core::serialize(out, m_dead);
  core::serialize(out, m_systemDbId);

  return out;
}

bool EntityRemovedMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_entityDbId);
  ok &= core::deserialize(in, m_entityKind);
  ok &= core::deserialize(in, m_dead);
  ok &= core::deserialize(in, m_systemDbId);

  return ok;
}

auto EntityRemovedMessage::clone() const -> bsgalone::core::IMessagePtr
{
  auto clone          = std::make_unique<EntityRemovedMessage>(m_entityDbId, m_entityKind, m_dead);
  clone->m_systemDbId = m_systemDbId;
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
