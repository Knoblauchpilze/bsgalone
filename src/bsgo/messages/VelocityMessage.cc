

#include "VelocityMessage.hh"
#include <core_utils/SerializationUtils.hh>

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

auto VelocityMessage::operator<<(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);

  out << m_shipEntityId;
  /// TODO: acceleration Eigen::Vector3f m_acceleration{};

  return out;
}

auto VelocityMessage::operator>>(std::istream &in) -> std::istream &
{
  utils::deserialize(in, m_messageType);

  in >> m_shipEntityId;
  /// TODO: acceleration Eigen::Vector3f m_acceleration{};

  return in;
}

} // namespace bsgo
