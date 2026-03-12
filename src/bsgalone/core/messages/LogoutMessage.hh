
#pragma once

#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class LogoutMessage : public NetworkMessage
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

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_playerDbId{};
  bool m_closeConnection{};

  std::optional<Uuid> m_systemDbId{};

  friend auto operator<<(std::ostream &out, const LogoutMessage &message) -> std::ostream &;
  friend auto operator>>(std::istream &in, LogoutMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const LogoutMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, LogoutMessage &message) -> std::istream &;

} // namespace bsgalone::core
