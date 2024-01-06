

#include "VelocityMessage.hh"

namespace bsgo {

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

} // namespace bsgo
