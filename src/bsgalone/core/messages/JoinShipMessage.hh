
#pragma once

#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class JoinShipMessage : public NetworkMessage
{
  public:
  JoinShipMessage();
  JoinShipMessage(const Uuid playerDbId, const Uuid shipDbId);
  ~JoinShipMessage() override = default;

  auto getPlayerDbId() const -> Uuid;
  auto getShipDbId() const -> Uuid;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_playerDbId{};
  Uuid m_shipDbId{};

  friend auto operator<<(std::ostream &out, const JoinShipMessage &message) -> std::ostream &;
  friend auto operator>>(std::istream &in, JoinShipMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const JoinShipMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, JoinShipMessage &message) -> std::istream &;

} // namespace bsgalone::core
