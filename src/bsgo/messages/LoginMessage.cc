
#include "LoginMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

LoginMessage::LoginMessage(const std::string &name, const std::string &password)
  : AbstractMessage(MessageType::LOGIN)
  , m_type(LoginType::LOGIN)
  , m_name(name)
  , m_password(password)
  , m_state(LoginState::PENDING)
{}

LoginMessage::LoginMessage(const std::string &name,
                           const std::string &password,
                           const Faction &faction)
  : AbstractMessage(MessageType::LOGIN)
  , m_type(LoginType::SIGNUP)
  , m_name(name)
  , m_password(password)
  , m_faction(faction)
  , m_state(LoginState::PENDING)
{}

LoginMessage::LoginMessage(const LoginType &type, const Uuid &playerId, const LoginState &state)
  : AbstractMessage(MessageType::LOGIN)
  , m_type(type)
  , m_state(state)
  , m_playerId(playerId)
{}

auto LoginMessage::getLoginType() const -> LoginType
{
  return m_type;
}

auto LoginMessage::getName() const -> std::string
{
  return m_name;
}

auto LoginMessage::getPassword() const -> std::string
{
  return m_password;
}

auto LoginMessage::getFaction() const -> std::optional<Faction>
{
  return m_faction;
}

auto LoginMessage::getLoginState() const -> LoginState
{
  return m_state;
}

auto LoginMessage::getPlayerId() const -> std::optional<Uuid>
{
  return m_playerId;
}

auto LoginMessage::operator<<(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);

  utils::serialize(out, m_type);
  utils::serialize(out, m_name);
  utils::serialize(out, m_password);
  utils::serialize(out, m_faction);

  utils::serialize(out, m_state);
  utils::serialize(out, m_playerId);

  return out;
}

auto LoginMessage::operator>>(std::istream &in) -> std::istream &
{
  utils::deserialize(in, m_messageType);

  utils::deserialize(in, m_type);
  utils::deserialize(in, m_name);
  utils::deserialize(in, m_password);
  utils::deserialize(in, m_faction);

  utils::deserialize(in, m_state);
  utils::deserialize(in, m_playerId);

  return in;
}

} // namespace bsgo
