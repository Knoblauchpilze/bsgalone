
#include "LogoutMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

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

auto LogoutMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_playerDbId);
  core::serialize(out, m_closeConnection);

  core::serialize(out, m_systemDbId);

  return out;
}

bool LogoutMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_playerDbId);
  ok &= core::deserialize(in, m_closeConnection);

  ok &= core::deserialize(in, m_systemDbId);

  return ok;
}

auto LogoutMessage::clone() const -> bsgalone::core::IMessagePtr
{
  auto clone = std::make_unique<LogoutMessage>(m_playerDbId, m_closeConnection);
  if (m_systemDbId)
  {
    clone->setSystemDbId(*m_systemDbId);
  }
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
