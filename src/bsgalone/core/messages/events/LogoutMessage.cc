
#include "LogoutMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

LogoutMessage::LogoutMessage()
  : IMessage(MessageType::LOGOUT)
{}

LogoutMessage::LogoutMessage(const Uuid playerDbId)
  : IMessage(MessageType::LOGOUT)
  , m_playerDbId(playerDbId)
{}

auto LogoutMessage::getPlayerDbId() const -> Uuid
{
  return m_playerDbId;
}

auto LogoutMessage::clone() const -> IMessagePtr
{
  return std::make_unique<LogoutMessage>(m_playerDbId);
}

auto LogoutMessage::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  LogoutMessage message;

  bool ok{true};
  ok &= ::core::deserialize(in, message.m_type);
  ok &= ::core::deserialize(in, message.m_playerDbId);

  if (!ok)
  {
    return {};
  }

  return message.clone();
}

auto operator<<(std::ostream &out, const LogoutMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_playerDbId);

  return out;
}

} // namespace bsgalone::core
