
#pragma once

#include "Uuid.hh"
#include "ValidatableMessage.hh"

namespace bsgo {

class LogoutMessage : public ValidatableMessage
{
  public:
  LogoutMessage();
  LogoutMessage(const Uuid playerDbId);
  LogoutMessage(const Uuid playerDbId, const bool closeConnection);
  ~LogoutMessage() override = default;

  auto getPlayerDbId() const -> Uuid;
  bool shouldCloseConnection() const;
  auto tryGetSystemDbId() const -> std::optional<Uuid>;

  void setSystemDbId(const Uuid systemDbId);

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_playerDbId{};
  bool m_closeConnection{};

  std::optional<Uuid> m_systemDbId{};
};

} // namespace bsgo
