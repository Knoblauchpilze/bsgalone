
#include "LogoutMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

LogoutMessage::LogoutMessage()
  : NetworkMessage(MessageType::LOGOUT)
{}

LogoutMessage::LogoutMessage(const Uuid playerDbId)
  : LogoutMessage(playerDbId, false)
{}

LogoutMessage::LogoutMessage(const Uuid playerDbId, const bool closeConnection)
  : NetworkMessage(MessageType::LOGOUT)
  , m_playerDbId(playerDbId)
  , m_closeConnection(closeConnection)
{}

auto LogoutMessage::getPlayerDbId() const -> Uuid
{
  return m_playerDbId;
}

bool LogoutMessage::shouldCloseConnection() const
{
  return m_closeConnection;
}

auto LogoutMessage::tryGetSystemDbId() const -> std::optional<Uuid>
{
  return m_systemDbId;
}

void LogoutMessage::setSystemDbId(const Uuid systemDbId)
{
  m_systemDbId = systemDbId;
}

auto LogoutMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<LogoutMessage>(m_playerDbId, m_closeConnection);
  if (m_systemDbId)
  {
    clone->setSystemDbId(*m_systemDbId);
  }
  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const LogoutMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_playerDbId);
  ::core::serialize(out, message.m_closeConnection);
  ::core::serialize(out, message.m_systemDbId);

  return out;
}

auto operator>>(std::istream &in, LogoutMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_playerDbId);
  ::core::deserialize(in, message.m_closeConnection);
  ::core::deserialize(in, message.m_systemDbId);

  return in;
}

} // namespace bsgalone::core
