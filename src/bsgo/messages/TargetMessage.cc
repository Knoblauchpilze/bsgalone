

#include "TargetMessage.hh"
#include "SerializationUtils.hh"
#include "VectorUtils.hh"

namespace bsgo {

TargetMessage::TargetMessage()
  : NetworkMessage(MessageType::TARGET)
{}

TargetMessage::TargetMessage(TargetData data, const Eigen::Vector3f &position)
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

auto TargetMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_data);
  core::serialize(out, m_systemDbId);
  core::serialize(out, m_position);

  return out;
}

bool TargetMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_data);
  ok &= core::deserialize(in, m_systemDbId);
  ok &= core::deserialize(in, m_position);

  return ok;
}

auto TargetMessage::clone() const -> bsgalone::core::IMessagePtr
{
  auto clone = std::make_unique<TargetMessage>(m_data, m_position);
  clone->copyClientIdIfDefined(*this);
  if (m_systemDbId)
  {
    clone->setSystemDbId(*m_systemDbId);
  }

  return clone;
}

} // namespace bsgo
