
#pragma once

#include "GameRole.hh"
#include "IUiEvent.hh"
#include <string>

namespace bsgalone::client {

class LoginCommand : public IUiEvent
{
  public:
  LoginCommand(const std::string &username, const std::string &password, const core::GameRole role);
  ~LoginCommand() override = default;

  auto getUsername() const -> std::string;
  auto getPassword() const -> std::string;
  auto getRole() const -> core::GameRole;

  auto clone() const -> IUiEventPtr override;

  private:
  std::string m_username{};
  std::string m_password{};
  core::GameRole m_role{};
};

} // namespace bsgalone::client
