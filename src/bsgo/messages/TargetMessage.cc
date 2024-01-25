

#include "TargetMessage.hh"
#include "VectorUtils.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

TargetMessage::TargetMessage()
  : NetworkMessage(MessageType::TARGET)
{}

TargetMessage::TargetMessage(const Uuid &shipEntityId, const Eigen::Vector3f &position)
  : NetworkMessage(MessageType::TARGET)
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
  utils::serialize(out, m_clientId);

  utils::serialize(out, m_shipEntityId);
  bsgo::serialize(out, m_position);

  return out;
}

bool TargetMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);

  ok &= utils::deserialize(in, m_shipEntityId);
  ok &= bsgo::deserialize(in, m_position);

  return ok;
}

} // namespace bsgo
