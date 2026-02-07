
#pragma once

#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class LogoutMessage : public NetworkMessage
{
  public:
  LogoutMessage();
  LogoutMessage(const Uuid playerDbId);
  LogoutMessage(const Uuid playerDbId, const bool closeConnection);
  ~LogoutMessage() override = default;

  auto getPlayerDbId() const -> Uuid;
  // TODO: This is not used anymore but should be somehow restored to make sure that
  // the player data attached to a client in the ClientManager is removed when a player
  // logs out without disconnecting from the server.
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
