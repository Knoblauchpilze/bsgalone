
#pragma once

#include "Faction.hh"
#include "NetworkMessage.hh"
#include "Uuid.hh"
#include <optional>
#include <string>

namespace bsgo {

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

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  std::string m_name{};
  std::string m_password{};
  Faction m_faction{};

  std::optional<Uuid> m_playerDbId{};
};

} // namespace bsgo
