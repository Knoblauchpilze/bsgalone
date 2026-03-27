
#pragma once

#include "Faction.hh"
#include "IUiCommand.hh"
#include <string>

namespace bsgalone::client {

class SignupCommand : public IUiCommand
{
  public:
  SignupCommand(const std::string &username,
                const std::string &password,
                const core::Faction faction);
  ~SignupCommand() override = default;

  auto getUsername() const -> std::string;
  auto getPassword() const -> std::string;
  auto getFaction() const -> core::Faction;

  auto clone() const -> IUiCommandPtr override;

  private:
  std::string m_username{};
  std::string m_password{};
  core::Faction m_faction{};
};

} // namespace bsgalone::client
