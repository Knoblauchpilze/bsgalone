
#include "JumpCancelledMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

JumpCancelledMessage::JumpCancelledMessage()
  : AbstractPlayerMessage(MessageType::JUMP_CANCELLED, bsgo::Uuid{0}, bsgo::Uuid{0})
{}

JumpCancelledMessage::JumpCancelledMessage(const bsgo::Uuid playerDbId,
                                           const bsgo::Uuid systemDbId,
                                           const bsgo::Uuid shipDbId)
  : AbstractPlayerMessage(MessageType::JUMP_CANCELLED, playerDbId, systemDbId)
  , m_shipDbId(shipDbId)
{}

auto JumpCancelledMessage::getShipDbId() const -> bsgo::Uuid
{
  return m_shipDbId;
}

auto JumpCancelledMessage::serialize(std::ostream &out) const -> std::ostream &
{
  ::core::serialize(out, m_messageType);
  ::core::serialize(out, m_playerDbId);
  ::core::serialize(out, m_systemDbId);

  ::core::serialize(out, m_shipDbId);

  return out;
}

bool JumpCancelledMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= ::core::deserialize(in, m_messageType);
  ok &= ::core::deserialize(in, m_playerDbId);
  ok &= ::core::deserialize(in, m_systemDbId);

  ok &= ::core::deserialize(in, m_shipDbId);

  return ok;
}

auto JumpCancelledMessage::clone() const -> IMessagePtr
{
  return std::make_unique<JumpCancelledMessage>(m_playerDbId, m_systemDbId, m_shipDbId);
}

auto JumpCancelledMessage::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  JumpCancelledMessage message;

  if (!message.deserialize(in))
  {
    return {};
  }

  return message.clone();
}

} // namespace bsgalone::core
