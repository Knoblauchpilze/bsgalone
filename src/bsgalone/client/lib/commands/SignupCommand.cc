
#include "SignupCommand.hh"

namespace bsgalone::client {

SignupCommand::SignupCommand(const std::string &username,
                             const std::string &password,
                             const core::Faction faction)
  : IUiCommand(UiCommandType::SIGNUP_REQUESTED)
  , m_username(username)
  , m_password(password)
  , m_faction(faction)
{}

auto SignupCommand::getUsername() const -> std::string
{
  return m_username;
}

auto SignupCommand::getPassword() const -> std::string
{
  return m_password;
}

auto SignupCommand::getFaction() const -> core::Faction
{
  return m_faction;
}

auto SignupCommand::clone() const -> IUiCommandPtr
{
  return std::make_unique<SignupCommand>(m_username, m_password, m_faction);
}

} // namespace bsgalone::client
