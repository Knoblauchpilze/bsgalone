
#include "LoginCommand.hh"

namespace bsgalone::client {

LoginCommand::LoginCommand(const std::string &username,
                           const std::string &password,
                           const core::GameRole role)
  : IUiCommand(UiCommandType::LOGIN_REQUESTED)
  , m_username(username)
  , m_password(password)
  , m_role(role)
{}

auto LoginCommand::getUsername() const -> std::string
{
  return m_username;
}

auto LoginCommand::getPassword() const -> std::string
{
  return m_password;
}

auto LoginCommand::getRole() const -> core::GameRole
{
  return m_role;
}

auto LoginCommand::clone() const -> IUiCommandPtr
{
  return std::make_unique<LoginCommand>(m_username, m_password, m_role);
}

} // namespace bsgalone::client
