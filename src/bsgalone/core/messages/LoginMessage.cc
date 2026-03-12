
#include "LoginMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

LoginMessage::LoginMessage()
  : NetworkMessage(MessageType::LOGIN)
{}

LoginMessage::LoginMessage(const GameRole role)
  : NetworkMessage(MessageType::LOGIN)
  , m_role(role)
{}

auto LoginMessage::getUserName() const -> std::string
{
  return m_name;
}

auto LoginMessage::getPassword() const -> std::string
{
  return m_password;
}

auto LoginMessage::getGameRole() const -> GameRole
{
  return m_role;
}

auto LoginMessage::tryGetPlayerDbId() const -> std::optional<Uuid>
{
  return m_playerDbId;
}

auto LoginMessage::tryGetSystemDbId() const -> std::optional<Uuid>
{
  return m_systemDbId;
}

void LoginMessage::setUserName(const std::string &userName)
{
  m_name = userName;
}

void LoginMessage::setPassword(const std::string &password)
{
  m_password = password;
}

void LoginMessage::setPlayerDbId(const Uuid playerDbId)
{
  m_playerDbId = playerDbId;
}

void LoginMessage::setSystemDbId(const Uuid systemDbId)
{
  m_systemDbId = systemDbId;
}

bool LoginMessage::successfullyLoggedIn() const
{
  return m_playerDbId.has_value();
}

auto LoginMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<LoginMessage>(m_role);
  clone->setUserName(m_name);
  clone->setPassword(m_password);
  if (m_playerDbId)
  {
    clone->setPlayerDbId(*m_playerDbId);
  }
  if (m_systemDbId)
  {
    clone->setSystemDbId(*m_systemDbId);
  }
  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const LoginMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_name);
  ::core::serialize(out, message.m_password);
  ::core::serialize(out, message.m_role);
  ::core::serialize(out, message.m_playerDbId);
  ::core::serialize(out, message.m_systemDbId);

  return out;
}

auto operator>>(std::istream &in, LoginMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_name);
  ::core::deserialize(in, message.m_password);
  ::core::deserialize(in, message.m_role);
  ::core::deserialize(in, message.m_playerDbId);
  ::core::deserialize(in, message.m_systemDbId);

  return in;
}

} // namespace bsgalone::core
