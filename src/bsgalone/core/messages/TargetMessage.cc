

#include "TargetMessage.hh"
#include "SerializationUtils.hh"
#include "VectorUtils.hh"

namespace bsgalone::core {

TargetMessage::TargetMessage()
  : NetworkMessage(MessageType::TARGET)
{}

TargetMessage::TargetMessage(Target data, const Eigen::Vector3f &position)
  : NetworkMessage(MessageType::TARGET)
  , m_data(std::move(data))
  , m_position(position)
{}

auto TargetMessage::getSourceDbId() const -> Uuid
{
  return m_data.sourceDbId;
}

auto TargetMessage::getSourceKind() const -> EntityKind
{
  return m_data.sourceKind;
}

auto TargetMessage::tryGetTargetDbId() const -> std::optional<Uuid>
{
  return m_data.targetDbId;
}

auto TargetMessage::tryGetTargetKind() const -> std::optional<EntityKind>
{
  return m_data.targetKind;
}

auto TargetMessage::tryGetSystemDbId() const -> std::optional<Uuid>
{
  return m_systemDbId;
}

void TargetMessage::setSystemDbId(const Uuid systemDbId)
{
  m_systemDbId = systemDbId;
}

auto TargetMessage::getPosition() const -> Eigen::Vector3f
{
  return m_position;
}

auto TargetMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<TargetMessage>(m_data, m_position);
  clone->copyClientIdIfDefined(*this);
  if (m_systemDbId)
  {
    clone->setSystemDbId(*m_systemDbId);
  }

  return clone;
}

auto operator<<(std::ostream &out, const TargetMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_data);
  ::core::serialize(out, message.m_systemDbId);
  ::core::serialize(out, message.m_position);

  return out;
}

auto operator>>(std::istream &in, TargetMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_data);
  ::core::deserialize(in, message.m_systemDbId);
  ::core::deserialize(in, message.m_position);

  return in;
}

} // namespace bsgalone::core
