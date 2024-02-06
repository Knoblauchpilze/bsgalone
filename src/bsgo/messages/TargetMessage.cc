

#include "TargetMessage.hh"
#include "VectorUtils.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

TargetMessage::TargetMessage()
  : ValidatableMessage(MessageType::TARGET)
{}

TargetMessage::TargetMessage(const Uuid shipDbId, const Eigen::Vector3f &position)
  : TargetMessage(shipDbId, position, {}, {})
{}

TargetMessage::TargetMessage(const Uuid shipDbId,
                             const Eigen::Vector3f &position,
                             const std::optional<EntityKind> &targetKind,
                             const std::optional<Uuid> &targetDbId)
  : ValidatableMessage(MessageType::TARGET)
  , m_shipDbId(shipDbId)
  , m_position(position)
  , m_targetKind(targetKind)
  , m_targetDbId(targetDbId)
{}

auto TargetMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto TargetMessage::getPosition() const -> Eigen::Vector3f
{
  return m_position;
}

auto TargetMessage::getTargetKind() const -> std::optional<EntityKind>
{
  return m_targetKind;
}

auto TargetMessage::getTargetDbId() const -> std::optional<Uuid>
{
  return m_targetDbId;
}

auto TargetMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);
  utils::serialize(out, m_validated);

  utils::serialize(out, m_shipDbId);
  bsgo::serialize(out, m_position);
  utils::serialize(out, m_targetKind);
  utils::serialize(out, m_targetDbId);

  return out;
}

bool TargetMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);
  ok &= utils::deserialize(in, m_validated);

  ok &= utils::deserialize(in, m_shipDbId);
  ok &= bsgo::deserialize(in, m_position);
  ok &= utils::deserialize(in, m_targetKind);
  ok &= utils::deserialize(in, m_targetDbId);

  return ok;
}

auto TargetMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<TargetMessage>(m_shipDbId, m_position, m_targetKind, m_targetDbId);
  clone->copyClientIdIfDefined(*this);
  clone->validate(validated());

  return clone;
}

} // namespace bsgo
