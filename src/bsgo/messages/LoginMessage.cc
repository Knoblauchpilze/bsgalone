
#include "LoginMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

LoginMessage::LoginMessage()
  : ValidatableMessage(MessageType::LOGIN)
{}

LoginMessage::LoginMessage(const std::string &name, const std::string &password)
  : LoginMessage(name, password, {})
{}

LoginMessage::LoginMessage(const std::string &name,
                           const std::string &password,
                           const std::optional<Uuid> &playerDbId)
  : ValidatableMessage(MessageType::LOGIN)
  , m_name(name)
  , m_password(password)
  , m_playerDbId(playerDbId)
{}

auto LoginMessage::getUserName() const -> std::string
{
  return m_name;
}

auto LoginMessage::getUserPassword() const -> std::string
{
  return m_password;
}

bool LoginMessage::successfullyLoggedIn() const
{
  return m_playerDbId.has_value();
}

auto LoginMessage::getPlayerDbId() const -> std::optional<Uuid>
{
  return m_playerDbId;
}

auto LoginMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);
  core::serialize(out, m_validated);

  core::serialize(out, m_name);
  core::serialize(out, m_password);

  core::serialize(out, m_playerDbId);

  return out;
}

bool LoginMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);
  ok &= core::deserialize(in, m_validated);

  ok &= core::deserialize(in, m_name);
  ok &= core::deserialize(in, m_password);

  ok &= core::deserialize(in, m_playerDbId);

  return ok;
}

auto LoginMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<LoginMessage>(m_name, m_password, m_playerDbId);
  clone->copyClientIdIfDefined(*this);
  clone->validate(validated());

  return clone;
}

} // namespace bsgo
