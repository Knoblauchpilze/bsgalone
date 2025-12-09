
#pragma once

#include "Faction.hh"
#include "Uuid.hh"
#include "ValidatableMessage.hh"
#include <optional>
#include <string>

namespace bsgo {

class LoginMessage : public ValidatableMessage
{
  public:
  LoginMessage();
  LoginMessage(const std::string &name, const std::string &password);
  ~LoginMessage() override = default;

  auto getUserName() const -> std::string;
  auto getUserPassword() const -> std::string;
  auto getPlayerDbId() const -> std::optional<Uuid>;

  void setPlayerDbId(const Uuid playerDbId);

  bool successfullyLoggedIn() const;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  std::string m_name{};
  std::string m_password{};

  std::optional<Uuid> m_playerDbId{};
};

} // namespace bsgo
