
#include "LockerDataMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

// TODO: Add tests for this class
// TODO: Add entries in the message parser/deseralizer
LockerDataMessage::LockerDataMessage()
  : IMessage(MessageType::LOCKER_DATA)
{}

LockerDataMessage::LockerDataMessage(const Uuid playerDbId)
  : IMessage(MessageType::LOCKER_DATA)
  , m_playerDbId(playerDbId)
{}

auto LockerDataMessage::getPlayerDbId() const -> Uuid
{
  return m_playerDbId;
}

auto LockerDataMessage::clone() const -> IMessagePtr
{
  return std::make_unique<LockerDataMessage>(m_playerDbId);
}

auto LockerDataMessage::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  LockerDataMessage message;

  bool ok{true};
  ok &= ::core::deserialize(in, message.m_type);
  ok &= ::core::deserialize(in, message.m_playerDbId);

  if (!ok)
  {
    return {};
  }

  return message.clone();
}

auto operator<<(std::ostream &out, const LockerDataMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_playerDbId);

  return out;
}

} // namespace bsgalone::core
