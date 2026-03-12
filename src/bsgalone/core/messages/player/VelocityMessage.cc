

#include "VelocityMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

VelocityMessage::VelocityMessage()
  : AbstractPlayerMessage(MessageType::VELOCITY, Uuid{0}, Uuid{0})
{}

VelocityMessage::VelocityMessage(const Uuid playerDbId,
                                 const Uuid systemDbId,
                                 const Uuid shipDbId,
                                 const Eigen::Vector3f &acceleration)
  : AbstractPlayerMessage(MessageType::VELOCITY, playerDbId, systemDbId)
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

auto VelocityMessage::clone() const -> IMessagePtr
{
  return std::make_unique<VelocityMessage>(m_playerDbId, m_systemDbId, m_shipDbId, m_acceleration);
}

auto VelocityMessage::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  VelocityMessage message;

  bool ok{true};
  ok &= ::core::deserialize(in, message.m_type);
  ok &= ::core::deserialize(in, message.m_playerDbId);
  ok &= ::core::deserialize(in, message.m_systemDbId);
  ok &= ::core::deserialize(in, message.m_shipDbId);
  ok &= ::core::deserialize(in, message.m_acceleration);

  if (!ok)
  {
    return {};
  }

  return message.clone();
}

auto operator<<(std::ostream &out, const VelocityMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_playerDbId);
  ::core::serialize(out, message.m_systemDbId);
  ::core::serialize(out, message.m_shipDbId);
  ::core::serialize(out, message.m_acceleration);

  return out;
}

} // namespace bsgalone::core
