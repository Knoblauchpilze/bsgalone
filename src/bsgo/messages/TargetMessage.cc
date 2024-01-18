

#include "TargetMessage.hh"
#include "VectorUtils.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

TargetMessage::TargetMessage(const Uuid &shipEntityId, const Eigen::Vector3f &position)
  : AbstractMessage(MessageType::TARGET)
  , m_shipEntityId(shipEntityId)
  , m_position(position)
{}

auto TargetMessage::getShipEntityId() const -> Uuid
{
  return m_shipEntityId;
}

auto TargetMessage::getPosition() const -> Eigen::Vector3f
{
  return m_position;
}

auto TargetMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);

  utils::serialize(out, m_shipEntityId);
  bsgo::serialize(out, m_position);

  return out;
}

bool TargetMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);

  ok &= utils::deserialize(in, m_shipEntityId);
  ok &= bsgo::deserialize(in, m_position);

  return ok;
}

} // namespace bsgo
