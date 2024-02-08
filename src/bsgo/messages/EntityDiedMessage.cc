
#include "EntityDiedMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

EntityDiedMessage::EntityDiedMessage()
  : ValidatableMessage(MessageType::ENTITY_DIED)
{}

EntityDiedMessage::EntityDiedMessage(const Uuid entityDbId, const EntityKind entityKind)
  : ValidatableMessage(MessageType::ENTITY_DIED)
  , m_entityDbId(entityDbId)
  , m_entityKind(entityKind)
{}

auto EntityDiedMessage::getEntityDbId() const -> Uuid
{
  return m_entityDbId;
}

auto EntityDiedMessage::getEntityKind() const -> EntityKind
{
  return m_entityKind;
}

auto EntityDiedMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);
  utils::serialize(out, m_validated);

  utils::serialize(out, m_entityDbId);
  utils::serialize(out, m_entityKind);

  return out;
}

bool EntityDiedMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);
  ok &= utils::deserialize(in, m_validated);

  ok &= utils::deserialize(in, m_entityDbId);
  ok &= utils::deserialize(in, m_entityKind);

  return ok;
}

auto EntityDiedMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<EntityDiedMessage>(m_entityDbId, m_entityKind);
  clone->copyClientIdIfDefined(*this);
  clone->validate(validated());

  return clone;
}

} // namespace bsgo
