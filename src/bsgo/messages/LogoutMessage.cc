
#include "LogoutMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

LogoutMessage::LogoutMessage()
  : ValidatableMessage(MessageType::LOGOUT)
{}

LogoutMessage::LogoutMessage(const Uuid playerDbId)
  : LogoutMessage(playerDbId, false)
{}

LogoutMessage::LogoutMessage(const Uuid playerDbId, const bool closeConnection)
  : ValidatableMessage(MessageType::LOGOUT)
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

auto LogoutMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);
  core::serialize(out, m_validated);

  core::serialize(out, m_playerDbId);
  core::serialize(out, m_closeConnection);

  return out;
}

bool LogoutMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);
  ok &= core::deserialize(in, m_validated);

  ok &= core::deserialize(in, m_playerDbId);
  ok &= core::deserialize(in, m_closeConnection);

  return ok;
}

auto LogoutMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<LogoutMessage>(m_playerDbId, m_closeConnection);
  clone->copyClientIdIfDefined(*this);
  clone->validate(validated());

  return clone;
}

} // namespace bsgo
