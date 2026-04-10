
#include "LogoutMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

LogoutMessage::LogoutMessage()
  : IMessage(MessageType::LOGOUT)
{}

auto LogoutMessage::clone() const -> IMessagePtr
{
  return std::make_unique<LogoutMessage>();
}

auto LogoutMessage::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  LogoutMessage message;

  bool ok{true};
  ok &= ::core::deserialize(in, message.m_type);

  if (!ok)
  {
    return {};
  }

  return message.clone();
}

auto operator<<(std::ostream &out, const LogoutMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);

  return out;
}

} // namespace bsgalone::core
