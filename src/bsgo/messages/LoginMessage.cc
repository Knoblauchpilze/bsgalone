
#include "LoginMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

LoginMessage::LoginMessage()
  : NetworkMessage(bsgalone::core::MessageType::LOGIN)
{}

LoginMessage::LoginMessage(const GameRole role)
  : NetworkMessage(bsgalone::core::MessageType::LOGIN)
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

auto LoginMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_name);
  core::serialize(out, m_password);
  core::serialize(out, m_role);

  core::serialize(out, m_playerDbId);
  core::serialize(out, m_systemDbId);

  return out;
}

bool LoginMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_name);
  ok &= core::deserialize(in, m_password);
  ok &= core::deserialize(in, m_role);

  ok &= core::deserialize(in, m_playerDbId);
  ok &= core::deserialize(in, m_systemDbId);

  return ok;
}

auto LoginMessage::clone() const -> bsgalone::core::IMessagePtr
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

} // namespace bsgo
