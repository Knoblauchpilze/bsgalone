
#include "JumpRequestedMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

JumpRequestedMessage::JumpRequestedMessage()
  : AbstractPlayerMessage(MessageType::JUMP_REQUESTED, bsgo::Uuid{0}, bsgo::Uuid{0})
{}

JumpRequestedMessage::JumpRequestedMessage(const bsgo::Uuid playerDbId,
                                           const bsgo::Uuid sourceSystemDbId,
                                           const bsgo::Uuid shipDbId,
                                           const bsgo::Uuid destinationSystemDbId)
  : AbstractPlayerMessage(MessageType::JUMP_REQUESTED, playerDbId, sourceSystemDbId)
  , m_shipDbId(shipDbId)
  , m_destinationSystemDbId(destinationSystemDbId)
{}

auto JumpRequestedMessage::getShipDbId() const -> bsgo::Uuid
{
  return m_shipDbId;
}

auto JumpRequestedMessage::getDestinationSystem() const -> bsgo::Uuid
{
  return m_destinationSystemDbId;
}

auto JumpRequestedMessage::serialize(std::ostream &out) const -> std::ostream &
{
  ::core::serialize(out, m_messageType);
  ::core::serialize(out, m_playerDbId);
  ::core::serialize(out, m_systemDbId);

  ::core::serialize(out, m_shipDbId);
  ::core::serialize(out, m_destinationSystemDbId);

  return out;
}

bool JumpRequestedMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= ::core::deserialize(in, m_messageType);
  ok &= ::core::deserialize(in, m_playerDbId);
  ok &= ::core::deserialize(in, m_systemDbId);

  ok &= ::core::deserialize(in, m_shipDbId);
  ok &= ::core::deserialize(in, m_destinationSystemDbId);

  return ok;
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

  if (!message.deserialize(in))
  {
    return {};
  }

  return message.clone();
}

} // namespace bsgalone::core
