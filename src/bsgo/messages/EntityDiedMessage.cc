
#include "EntityDiedMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

EntityDiedMessage::EntityDiedMessage()
  : NetworkMessage(MessageType::ENTITY_DIED)
{}

EntityDiedMessage::EntityDiedMessage(const Uuid entityDbId, const EntityKind entityKind)
  : NetworkMessage(MessageType::ENTITY_DIED)
  , m_entityDbId(entityDbId)
  , m_entityKind(entityKind)
{}

EntityDiedMessage::EntityDiedMessage(const Uuid entityDbId,
                                     const EntityKind entityKind,
                                     const Uuid systemDbId)
  : NetworkMessage(MessageType::ENTITY_DIED)
  , m_entityDbId(entityDbId)
  , m_entityKind(entityKind)
  , m_systemDbId(systemDbId)
{}

auto EntityDiedMessage::getEntityDbId() const -> Uuid
{
  return m_entityDbId;
}

auto EntityDiedMessage::getEntityKind() const -> EntityKind
{
  return m_entityKind;
}

auto EntityDiedMessage::tryGetSystemDbId() const -> std::optional<Uuid>
{
  return m_systemDbId;
}

auto EntityDiedMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);

  utils::serialize(out, m_entityDbId);
  utils::serialize(out, m_entityKind);
  utils::serialize(out, m_systemDbId);

  return out;
}

bool EntityDiedMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);

  ok &= utils::deserialize(in, m_entityDbId);
  ok &= utils::deserialize(in, m_entityKind);
  ok &= utils::deserialize(in, m_systemDbId);

  return ok;
}

auto EntityDiedMessage::clone() const -> IMessagePtr
{
  auto clone          = std::make_unique<EntityDiedMessage>(m_entityDbId, m_entityKind);
  clone->m_systemDbId = m_systemDbId;
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
