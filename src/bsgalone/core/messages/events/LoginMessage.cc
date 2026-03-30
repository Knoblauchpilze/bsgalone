
#include "LoginMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

LoginMessage::LoginMessage()
  : IMessage(MessageType::LOGIN)
{}

LoginMessage::LoginMessage(const net::ClientId clientId)
  : IMessage(MessageType::LOGIN)
  , m_clientId(clientId)
{}

auto LoginMessage::getClientId() const -> net::ClientId
{
  return m_clientId;
}

bool LoginMessage::successfullyLoggedIn() const
{
  return m_playerDbId.has_value() && m_role.has_value() && m_systemDbId.has_value();
}

auto LoginMessage::getPlayerDbId() const -> Uuid
{
  return m_playerDbId.value();
}

auto LoginMessage::getRole() const -> GameRole
{
  return m_role.value();
}

auto LoginMessage::getSystemDbId() const -> Uuid
{
  return m_systemDbId.value();
}

void LoginMessage::setPlayerDbId(const Uuid playerDbId)
{
  m_playerDbId = playerDbId;
}

void LoginMessage::setRole(const GameRole role)
{
  m_role = role;
}

void LoginMessage::setSystemDbId(const Uuid systemDbId)
{
  m_systemDbId = systemDbId;
}

auto LoginMessage::clone() const -> IMessagePtr
{
  auto clone          = std::make_unique<LoginMessage>(m_clientId);
  clone->m_playerDbId = m_playerDbId;
  clone->m_role       = m_role;
  clone->m_systemDbId = m_systemDbId;

  return clone;
}

auto LoginMessage::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  LoginMessage message;

  bool ok{true};
  ok &= ::core::deserialize(in, message.m_type);
  ok &= ::core::deserialize(in, message.m_clientId);
  ok &= ::core::deserialize(in, message.m_playerDbId);
  ok &= ::core::deserialize(in, message.m_role);
  ok &= ::core::deserialize(in, message.m_systemDbId);

  if (!ok)
  {
    return {};
  }

  return message.clone();
}

auto operator<<(std::ostream &out, const LoginMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_playerDbId);
  ::core::serialize(out, message.m_role);
  ::core::serialize(out, message.m_systemDbId);

  return out;
}

} // namespace bsgalone::core
