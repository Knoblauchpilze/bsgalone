

#include "VelocityMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

VelocityMessage::VelocityMessage()
  : AbstractPlayerMessage(MessageType::VELOCITY, bsgo::Uuid{0}, bsgo::Uuid{0})
{}

VelocityMessage::VelocityMessage(const bsgo::Uuid playerDbId,
                                 const bsgo::Uuid systemDbId,
                                 const bsgo::Uuid shipDbId,
                                 const Eigen::Vector3f &acceleration)
  : AbstractPlayerMessage(MessageType::VELOCITY, playerDbId, systemDbId)
  , m_shipDbId(shipDbId)
  , m_acceleration(acceleration)
{}

auto VelocityMessage::getShipDbId() const -> bsgo::Uuid
{
  return m_shipDbId;
}

auto VelocityMessage::getAcceleration() const -> Eigen::Vector3f
{
  return m_acceleration;
}

auto VelocityMessage::serialize(std::ostream &out) const -> std::ostream &
{
  ::core::serialize(out, m_messageType);
  ::core::serialize(out, m_playerDbId);
  ::core::serialize(out, m_systemDbId);

  ::core::serialize(out, m_shipDbId);
  ::core::serialize(out, m_acceleration);

  return out;
}

bool VelocityMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= ::core::deserialize(in, m_messageType);
  ok &= ::core::deserialize(in, m_playerDbId);
  ok &= ::core::deserialize(in, m_systemDbId);

  ok &= ::core::deserialize(in, m_shipDbId);
  ok &= ::core::deserialize(in, m_acceleration);

  return ok;
}

auto VelocityMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<VelocityMessage>(m_playerDbId,
                                                 m_systemDbId,
                                                 m_shipDbId,
                                                 m_acceleration);

  return clone;
}

auto VelocityMessage::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  VelocityMessage message;

  bool ok{true};
  ok &= ::core::deserialize(in, message.m_messageType);
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

} // namespace bsgalone::core
