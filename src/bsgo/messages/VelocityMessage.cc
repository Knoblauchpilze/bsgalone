

#include "VelocityMessage.hh"
#include "SerializationUtils.hh"
#include "VectorUtils.hh"

namespace bsgo {

VelocityMessage::VelocityMessage()
  : NetworkMessage(bsgalone::core::MessageType::VELOCITY)
{}

VelocityMessage::VelocityMessage(const Uuid shipDbId, const Eigen::Vector3f &acceleration)
  : NetworkMessage(bsgalone::core::MessageType::VELOCITY)
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
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_shipDbId);
  core::serialize(out, m_acceleration);

  return out;
}

bool VelocityMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_shipDbId);
  ok &= core::deserialize(in, m_acceleration);

  return ok;
}

auto VelocityMessage::clone() const -> bsgalone::core::IMessagePtr
{
  auto clone = std::make_unique<VelocityMessage>(m_shipDbId, m_acceleration);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
