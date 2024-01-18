
#pragma once

#include "AbstractMessage.hh"
#include "Faction.hh"
#include "Uuid.hh"
#include <optional>
#include <string>

namespace bsgo {

enum class LoginType
{
  LOGIN,
  SIGNUP
};

enum class LoginState
{
  PENDING,
  VALIDATED,
  REJECTED
};

class LoginMessage : public AbstractMessage
{
  public:
  LoginMessage();
  LoginMessage(const std::string &name, const std::string &password);
  LoginMessage(const std::string &name, const std::string &password, const Faction &faction);
  LoginMessage(const LoginType &type, const Uuid &playerId, const LoginState &state);
  ~LoginMessage() override = default;

  auto getLoginType() const -> LoginType;
  auto getUserName() const -> std::string;
  auto getUserPassword() const -> std::string;
  auto getFaction() const -> std::optional<Faction>;

  auto getLoginState() const -> LoginState;
  auto getPlayerId() const -> std::optional<Uuid>;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  private:
  LoginType m_type{};
  std::string m_name{};
  std::string m_password{};
  std::optional<Faction> m_faction{};

  LoginState m_state{LoginState::PENDING};
  std::optional<Uuid> m_playerId{};
};

} // namespace bsgo
