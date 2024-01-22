
#include "LoginMessage.hh"
#include <core_utils/SerializationUtils.hh>

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
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_validated);

  utils::serialize(out, m_name);
  utils::serialize(out, m_password);

  utils::serialize(out, m_playerDbId);

  return out;
}

bool LoginMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_validated);

  ok &= utils::deserialize(in, m_name);
  ok &= utils::deserialize(in, m_password);

  ok &= utils::deserialize(in, m_playerDbId);

  return ok;
}

} // namespace bsgo
