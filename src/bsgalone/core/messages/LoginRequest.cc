
#include "LoginRequest.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

// TODO: Add tests for this class
LoginRequest::LoginRequest()
  : IMessage(MessageType::LOGIN_REQUEST)
{}

LoginRequest::LoginRequest(const std::string &username,
                           const std::string &password,
                           const GameRole role)
  : IMessage(MessageType::LOGIN_REQUEST)
  , m_username(username)
  , m_password(password)
  , m_role(role)
{}

auto LoginRequest::getUsername() const -> std::string
{
  return m_username;
}

auto LoginRequest::getPassword() const -> std::string
{
  return m_password;
}

auto LoginRequest::getRole() const -> GameRole
{
  return m_role;
}

auto LoginRequest::tryGetClientId() const -> std::optional<net::ClientId>
{
  return m_clientId;
}

void LoginRequest::setClientId(const net::ClientId clientId)
{
  m_clientId = clientId;
}

auto LoginRequest::clone() const -> IMessagePtr
{
  auto out = std::make_unique<LoginRequest>(m_username, m_password, m_role);
  if (m_clientId.has_value())
  {
    out->setClientId(*m_clientId);
  }

  return out;
}

auto LoginRequest::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  LoginRequest message;

  bool ok{true};
  ok &= ::core::deserialize(in, message.m_type);
  ok &= ::core::deserialize(in, message.m_username);
  ok &= ::core::deserialize(in, message.m_password);
  ok &= ::core::deserialize(in, message.m_role);
  ok &= ::core::deserialize(in, message.m_clientId);

  if (!ok)
  {
    return {};
  }

  return message.clone();
}

auto operator<<(std::ostream &out, const LoginRequest &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_username);
  ::core::serialize(out, message.m_password);
  ::core::serialize(out, message.m_role);
  ::core::serialize(out, message.m_clientId);

  return out;
}

} // namespace bsgalone::core
