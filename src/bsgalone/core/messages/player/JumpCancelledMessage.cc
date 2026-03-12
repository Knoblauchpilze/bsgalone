
#include "JumpCancelledMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

JumpCancelledMessage::JumpCancelledMessage()
  : AbstractPlayerMessage(MessageType::JUMP_CANCELLED, Uuid{0}, Uuid{0})
{}

JumpCancelledMessage::JumpCancelledMessage(const Uuid playerDbId,
                                           const Uuid systemDbId,
                                           const Uuid shipDbId)
  : AbstractPlayerMessage(MessageType::JUMP_CANCELLED, playerDbId, systemDbId)
  , m_shipDbId(shipDbId)
{}

auto JumpCancelledMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto JumpCancelledMessage::clone() const -> IMessagePtr
{
  return std::make_unique<JumpCancelledMessage>(m_playerDbId, m_systemDbId, m_shipDbId);
}

auto JumpCancelledMessage::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  JumpCancelledMessage message;

  bool ok{true};
  ok &= ::core::deserialize(in, message.m_type);
  ok &= ::core::deserialize(in, message.m_playerDbId);
  ok &= ::core::deserialize(in, message.m_systemDbId);
  ok &= ::core::deserialize(in, message.m_shipDbId);

  if (!ok)
  {
    return {};
  }

  return message.clone();
}

auto operator<<(std::ostream &out, const JumpCancelledMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_playerDbId);
  ::core::serialize(out, message.m_systemDbId);
  ::core::serialize(out, message.m_shipDbId);

  return out;
}

} // namespace bsgalone::core
