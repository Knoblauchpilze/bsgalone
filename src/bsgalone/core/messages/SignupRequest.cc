
#include "SignupRequest.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

SignupRequest::SignupRequest()
  : IMessage(MessageType::SIGNUP_REQUEST)
{}

SignupRequest::SignupRequest(const std::string &username,
                             const std::string &password,
                             const Faction faction)
  : IMessage(MessageType::SIGNUP_REQUEST)
  , m_username(username)
  , m_password(password)
  , m_faction(faction)
{}

auto SignupRequest::getUsername() const -> std::string
{
  return m_username;
}

auto SignupRequest::getPassword() const -> std::string
{
  return m_password;
}

auto SignupRequest::getFaction() const -> Faction
{
  return m_faction;
}

auto SignupRequest::tryGetClientId() const -> std::optional<net::ClientId>
{
  return m_clientId;
}

void SignupRequest::setClientId(const net::ClientId clientId)
{
  m_clientId = clientId;
}

auto SignupRequest::clone() const -> IMessagePtr
{
  auto out = std::make_unique<SignupRequest>(m_username, m_password, m_faction);
  if (m_clientId.has_value())
  {
    out->setClientId(*m_clientId);
  }

  return out;
}

auto SignupRequest::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  SignupRequest message;

  bool ok{true};
  ok &= ::core::deserialize(in, message.m_type);
  ok &= ::core::deserialize(in, message.m_username);
  ok &= ::core::deserialize(in, message.m_password);
  ok &= ::core::deserialize(in, message.m_faction);
  ok &= ::core::deserialize(in, message.m_clientId);

  if (!ok)
  {
    return {};
  }

  return message.clone();
}

auto operator<<(std::ostream &out, const SignupRequest &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_username);
  ::core::serialize(out, message.m_password);
  ::core::serialize(out, message.m_faction);
  ::core::serialize(out, message.m_clientId);

  return out;
}

} // namespace bsgalone::core
