
#pragma once

#include "Faction.hh"
#include "GameRole.hh"
#include "NetworkMessage.hh"
#include "Uuid.hh"
#include <optional>
#include <string>

namespace bsgalone::core {

class LoginMessage : public NetworkMessage
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

  auto clone() const -> IMessagePtr override;

  private:
  std::string m_name{};
  std::string m_password{};
  GameRole m_role{};

  std::optional<Uuid> m_playerDbId{};
  std::optional<Uuid> m_systemDbId{};

  friend auto operator<<(std::ostream &out, const LoginMessage &message) -> std::ostream &;
  friend auto operator>>(std::istream &in, LoginMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const LoginMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, LoginMessage &message) -> std::istream &;

} // namespace bsgalone::core
