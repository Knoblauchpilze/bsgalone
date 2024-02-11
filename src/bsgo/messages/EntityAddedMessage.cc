
#include "EntityAddedMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

EntityAddedMessage::EntityAddedMessage()
  : NetworkMessage(MessageType::ENTITY_ADDED)
{}

EntityAddedMessage::EntityAddedMessage(const Uuid entityDbId,
                                       const EntityKind entityKind,
                                       const Uuid systemDbId)
  : NetworkMessage(MessageType::ENTITY_ADDED)
  , m_entityDbId(entityDbId)
  , m_entityKind(entityKind)
  , m_systemDbId(systemDbId)
{}

auto EntityAddedMessage::getEntityDbId() const -> Uuid
{
  return m_entityDbId;
}

auto EntityAddedMessage::getEntityKind() const -> EntityKind
{
  return m_entityKind;
}

auto EntityAddedMessage::getSystemDbId() const -> Uuid
{
  return m_systemDbId;
}

auto EntityAddedMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);

  utils::serialize(out, m_entityDbId);
  utils::serialize(out, m_entityKind);
  utils::serialize(out, m_systemDbId);

  return out;
}

bool EntityAddedMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);

  ok &= utils::deserialize(in, m_entityDbId);
  ok &= utils::deserialize(in, m_entityKind);
  ok &= utils::deserialize(in, m_systemDbId);

  return ok;
}

auto EntityAddedMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<EntityAddedMessage>(m_entityDbId, m_entityKind, m_systemDbId);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
