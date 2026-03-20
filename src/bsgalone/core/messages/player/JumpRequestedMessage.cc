
#include "JumpRequestedMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

JumpRequestedMessage::JumpRequestedMessage()
  : AbstractPlayerCommand(MessageType::JUMP_REQUESTED, Uuid{0}, Uuid{0})
{}

JumpRequestedMessage::JumpRequestedMessage(const Uuid playerDbId,
                                           const Uuid sourceSystemDbId,
                                           const Uuid shipDbId,
                                           const Uuid destinationSystemDbId)
  : AbstractPlayerCommand(MessageType::JUMP_REQUESTED, playerDbId, sourceSystemDbId)
  , m_shipDbId(shipDbId)
  , m_destinationSystemDbId(destinationSystemDbId)
{}

auto JumpRequestedMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto JumpRequestedMessage::getDestinationSystem() const -> Uuid
{
  return m_destinationSystemDbId;
}

auto JumpRequestedMessage::clone() const -> IMessagePtr
{
  return std::make_unique<JumpRequestedMessage>(m_playerDbId,
                                                m_systemDbId,
                                                m_shipDbId,
                                                m_destinationSystemDbId);
}

auto JumpRequestedMessage::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  JumpRequestedMessage message;

  bool ok{true};
  ok &= ::core::deserialize(in, message.m_type);
  ok &= ::core::deserialize(in, message.m_playerDbId);
  ok &= ::core::deserialize(in, message.m_systemDbId);
  ok &= ::core::deserialize(in, message.m_shipDbId);
  ok &= ::core::deserialize(in, message.m_destinationSystemDbId);

  if (!ok)
  {
    return {};
  }

  return message.clone();
}

auto operator<<(std::ostream &out, const JumpRequestedMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_playerDbId);
  ::core::serialize(out, message.m_systemDbId);
  ::core::serialize(out, message.m_shipDbId);
  ::core::serialize(out, message.m_destinationSystemDbId);

  return out;
}

} // namespace bsgalone::core
