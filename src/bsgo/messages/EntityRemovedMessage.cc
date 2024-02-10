
#include "EntityRemovedMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

EntityRemovedMessage::EntityRemovedMessage()
  : NetworkMessage(MessageType::ENTITY_REMOVED)
{}

EntityRemovedMessage::EntityRemovedMessage(const Uuid entityDbId, const EntityKind entityKind)
  : NetworkMessage(MessageType::ENTITY_REMOVED)
  , m_entityDbId(entityDbId)
  , m_entityKind(entityKind)
{}

EntityRemovedMessage::EntityRemovedMessage(const Uuid entityDbId,
                                           const EntityKind entityKind,
                                           const Uuid systemDbId)
  : NetworkMessage(MessageType::ENTITY_REMOVED)
  , m_entityDbId(entityDbId)
  , m_entityKind(entityKind)
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
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);

  utils::serialize(out, m_entityDbId);
  utils::serialize(out, m_entityKind);
  utils::serialize(out, m_systemDbId);

  return out;
}

bool EntityRemovedMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);

  ok &= utils::deserialize(in, m_entityDbId);
  ok &= utils::deserialize(in, m_entityKind);
  ok &= utils::deserialize(in, m_systemDbId);

  return ok;
}

auto EntityRemovedMessage::clone() const -> IMessagePtr
{
  auto clone          = std::make_unique<EntityRemovedMessage>(m_entityDbId, m_entityKind);
  clone->m_systemDbId = m_systemDbId;
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
