
#pragma once

#include "Faction.hh"
#include "NetworkMessage.hh"
#include "Uuid.hh"
#include <optional>
#include <string>

namespace bsgalone::core {

class SignupMessage : public NetworkMessage
{
  public:
  SignupMessage();
  SignupMessage(const std::string &name, const std::string &password, const Faction &faction);
  SignupMessage(const std::string &name,
                const std::string &password,
                const Faction &faction,
                const std::optional<Uuid> &playerDbId);
  ~SignupMessage() override = default;

  auto getUserName() const -> std::string;
  auto getUserPassword() const -> std::string;
  auto getFaction() const -> Faction;

  bool successfullySignedup() const;
  auto getPlayerDbId() const -> std::optional<Uuid>;

  auto clone() const -> IMessagePtr override;

  private:
  std::string m_name{};
  std::string m_password{};
  Faction m_faction{};

  std::optional<Uuid> m_playerDbId{};

  friend auto operator<<(std::ostream &out, const SignupMessage &message) -> std::ostream &;
  friend auto operator>>(std::istream &in, SignupMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const SignupMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, SignupMessage &message) -> std::istream &;

} // namespace bsgalone::core
