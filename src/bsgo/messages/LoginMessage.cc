
#include "LoginMessage.hh"

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

} // namespace bsgo
