
#include "ComponentSyncMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

ComponentSyncMessage::ComponentSyncMessage()
  : NetworkMessage(MessageType::COMPONENT_SYNC)
{}

ComponentSyncMessage::ComponentSyncMessage(const Uuid entityDbId,
                                           const EntityKind entityKind,
                                           const Uuid systemDbId)
  : NetworkMessage(MessageType::COMPONENT_SYNC)
  , m_entityDbId(entityDbId)
  , m_entityKind(entityKind)
  , m_systemDbId(systemDbId)
{}

auto ComponentSyncMessage::getEntityDbId() const -> Uuid
{
  return m_entityDbId;
}

auto ComponentSyncMessage::getEntityKind() const -> EntityKind
{
  return m_entityKind;
}

auto ComponentSyncMessage::getSystemDbId() const -> Uuid
{
  return m_systemDbId;
}

void ComponentSyncMessage::setStatus(const Status status)
{
  m_status = status;
}

auto ComponentSyncMessage::tryGetStatus() const -> std::optional<Status>
{
  return m_status;
}

auto ComponentSyncMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);

  utils::serialize(out, m_entityDbId);
  utils::serialize(out, m_entityKind);
  utils::serialize(out, m_systemDbId);

  utils::serialize(out, m_status);

  return out;
}

bool ComponentSyncMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);

  ok &= utils::deserialize(in, m_entityDbId);
  ok &= utils::deserialize(in, m_entityKind);
  ok &= utils::deserialize(in, m_systemDbId);

  ok &= utils::deserialize(in, m_status);

  return ok;
}

auto ComponentSyncMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<ComponentSyncMessage>(m_entityDbId, m_entityKind, m_systemDbId);
  clone->copyClientIdIfDefined(*this);

  clone->m_status = m_status;

  return clone;
}

} // namespace bsgo
