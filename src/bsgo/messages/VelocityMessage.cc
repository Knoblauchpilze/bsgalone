

#include "VelocityMessage.hh"
#include "VectorUtils.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

VelocityMessage::VelocityMessage()
  : AbstractMessage(MessageType::VELOCITY)
{}

VelocityMessage::VelocityMessage(const Uuid &shipEntityId, const Eigen::Vector3f &acceleration)
  : AbstractMessage(MessageType::VELOCITY)
  , m_shipEntityId(shipEntityId)
  , m_acceleration(acceleration)
{}

auto VelocityMessage::getShipEntityId() const -> Uuid
{
  return m_shipEntityId;
}

auto VelocityMessage::getAcceleration() const -> Eigen::Vector3f
{
  return m_acceleration;
}

auto VelocityMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);

  utils::serialize(out, m_shipEntityId);
  bsgo::serialize(out, m_acceleration);

  return out;
}

bool VelocityMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);

  ok &= utils::deserialize(in, m_shipEntityId);
  ok &= bsgo::deserialize(in, m_acceleration);

  return ok;
}

} // namespace bsgo
