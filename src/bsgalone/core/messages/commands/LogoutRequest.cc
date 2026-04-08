
#include "LogoutRequest.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

LogoutRequest::LogoutRequest()
  : IMessage(MessageType::LOGOUT_REQUEST)
{}

LogoutRequest::LogoutRequest(const Uuid playerDbId)
  : IMessage(MessageType::LOGOUT_REQUEST)
  , m_playerDbId(playerDbId)
{}

auto LogoutRequest::getPlayerDbId() const -> Uuid
{
  return m_playerDbId;
}

auto LogoutRequest::clone() const -> IMessagePtr
{
  return std::make_unique<LogoutRequest>(m_playerDbId);
}

auto LogoutRequest::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  LogoutRequest message;

  bool ok{true};
  ok &= ::core::deserialize(in, message.m_type);
  ok &= ::core::deserialize(in, message.m_playerDbId);

  if (!ok)
  {
    return {};
  }

  return message.clone();
}

auto operator<<(std::ostream &out, const LogoutRequest &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_playerDbId);

  return out;
}

} // namespace bsgalone::core
