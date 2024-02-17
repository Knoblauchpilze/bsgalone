
#include "ComponentSyncMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

ComponentSyncMessage::ComponentSyncMessage()
  : NetworkMessage(MessageType::COMPONENT_SYNC)
{}

ComponentSyncMessage::ComponentSyncMessage(const Uuid entityDbId, const EntityKind entityKind)
  : NetworkMessage(MessageType::COMPONENT_SYNC)
  , m_entityDbId(entityDbId)
  , m_entityKind(entityKind)
{}

auto ComponentSyncMessage::getEntityDbId() const -> Uuid
{
  return m_entityDbId;
}

auto ComponentSyncMessage::getEntityKind() const -> EntityKind
{
  return m_entityKind;
}

void ComponentSyncMessage::setSystemDbId(const Uuid systemDbId)
{
  m_systemDbId = systemDbId;
}

auto ComponentSyncMessage::tryGetSystemDbId() const -> std::optional<Uuid>
{
  return m_systemDbId;
}

auto ComponentSyncMessage::getSystemDbId() const -> Uuid
{
  if (!m_systemDbId)
  {
    error("Expected system db id to be defined but it was not");
  }
  return *m_systemDbId;
}

void ComponentSyncMessage::setStatus(const Status status)
{
  m_status = status;
}

auto ComponentSyncMessage::tryGetStatus() const -> std::optional<Status>
{
  return m_status;
}

void ComponentSyncMessage::setPosition(const Eigen::Vector3f &position)
{
  m_position = position;
}

auto ComponentSyncMessage::tryGetPosition() const -> std::optional<Eigen::Vector3f>
{
  return m_position;
}

void ComponentSyncMessage::setSpeed(const Eigen::Vector3f &speed)
{
  m_speed = speed;
}

auto ComponentSyncMessage::tryGetSpeed() const -> std::optional<Eigen::Vector3f>
{
  return m_speed;
}

void ComponentSyncMessage::setAcceleration(const Eigen::Vector3f &acceleration)
{
  m_acceleration = acceleration;
}

auto ComponentSyncMessage::tryGetAcceleration() const -> std::optional<Eigen::Vector3f>
{
  return m_acceleration;
}

auto ComponentSyncMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);

  utils::serialize(out, m_entityDbId);
  utils::serialize(out, m_entityKind);

  utils::serialize(out, m_systemDbId);
  utils::serialize(out, m_status);
  utils::serialize(out, m_position);
  utils::serialize(out, m_speed);
  utils::serialize(out, m_acceleration);

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
  ok &= utils::deserialize(in, m_position);
  ok &= utils::deserialize(in, m_speed);
  ok &= utils::deserialize(in, m_acceleration);

  return ok;
}

auto ComponentSyncMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<ComponentSyncMessage>(m_entityDbId, m_entityKind);
  clone->copyClientIdIfDefined(*this);

  clone->m_systemDbId   = m_systemDbId;
  clone->m_status       = m_status;
  clone->m_position     = m_position;
  clone->m_speed        = m_speed;
  clone->m_acceleration = m_acceleration;

  return clone;
}

} // namespace bsgo
