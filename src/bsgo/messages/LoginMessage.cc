
#include "LoginMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

LoginMessage::LoginMessage()
  : ValidatableMessage(MessageType::LOGIN)
{}

LoginMessage::LoginMessage(const std::string &name, const std::string &password, const GameRole role)
  : ValidatableMessage(MessageType::LOGIN)
  , m_name(name)
  , m_password(password)
  , m_role(role)
{}

auto LoginMessage::getUserName() const -> std::string
{
  return m_name;
}

auto LoginMessage::getUserPassword() const -> std::string
{
  return m_password;
}

auto LoginMessage::getGameRole() const -> GameRole
{
  return m_role;
}

auto LoginMessage::getPlayerDbId() const -> std::optional<Uuid>
{
  return m_playerDbId;
}

void LoginMessage::setPlayerDbId(const Uuid playerDbId)
{
  m_playerDbId = playerDbId;
}

bool LoginMessage::successfullyLoggedIn() const
{
  return m_playerDbId.has_value();
}

auto LoginMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);
  core::serialize(out, m_validated);

  core::serialize(out, m_name);
  core::serialize(out, m_password);
  core::serialize(out, m_role);

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
  ok &= core::deserialize(in, m_role);

  ok &= core::deserialize(in, m_playerDbId);

  return ok;
}

auto LoginMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<LoginMessage>(m_name, m_password, m_role);
  if (m_playerDbId)
  {
    clone->setPlayerDbId(*m_playerDbId);
  }
  clone->copyClientIdIfDefined(*this);
  clone->validate(validated());

  return clone;
}

} // namespace bsgo
