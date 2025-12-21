
#pragma once

#include "Faction.hh"
#include "GameRole.hh"
#include "Uuid.hh"
#include "ValidatableMessage.hh"
#include <optional>
#include <string>

namespace bsgo {

class LoginMessage : public ValidatableMessage
{
  public:
  LoginMessage();
  LoginMessage(const GameRole role);
  ~LoginMessage() override = default;

  auto getUserName() const -> std::string;
  auto getPassword() const -> std::string;
  auto getGameRole() const -> GameRole;
  auto tryGetPlayerDbId() const -> std::optional<Uuid>;
  auto tryGetSystemDbId() const -> std::optional<Uuid>;

  void setUserName(const std::string &userName);
  void setPassword(const std::string &password);
  void setPlayerDbId(const Uuid playerDbId);
  void setSystemDbId(const Uuid systemDbId);

  bool successfullyLoggedIn() const;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  std::string m_name{};
  std::string m_password{};
  GameRole m_role{};

  std::optional<Uuid> m_playerDbId{};
  std::optional<Uuid> m_systemDbId{};
};

} // namespace bsgo
