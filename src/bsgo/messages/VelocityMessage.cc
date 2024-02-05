

#include "VelocityMessage.hh"
#include "VectorUtils.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

VelocityMessage::VelocityMessage()
  : NetworkMessage(MessageType::VELOCITY)
{}

VelocityMessage::VelocityMessage(const Uuid shipDbId, const Eigen::Vector3f &acceleration)
  : NetworkMessage(MessageType::VELOCITY)
  , m_shipDbId(shipDbId)
  , m_acceleration(acceleration)
{}

auto VelocityMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto VelocityMessage::getAcceleration() const -> Eigen::Vector3f
{
  return m_acceleration;
}

auto VelocityMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);

  utils::serialize(out, m_shipDbId);
  bsgo::serialize(out, m_acceleration);

  return out;
}

bool VelocityMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);

  ok &= utils::deserialize(in, m_shipDbId);
  ok &= bsgo::deserialize(in, m_acceleration);

  return ok;
}

auto VelocityMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<VelocityMessage>(m_shipDbId, m_acceleration);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
