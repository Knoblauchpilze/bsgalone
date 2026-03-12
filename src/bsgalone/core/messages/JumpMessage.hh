
#pragma once

#include "NetworkMessage.hh"
#include "Uuid.hh"
#include <optional>

namespace bsgalone::core {

class JumpMessage : public NetworkMessage
{
  public:
  JumpMessage();
  JumpMessage(const Uuid shipDbId, const Uuid playerDbId);
  JumpMessage(const Uuid shipDbId,
              const Uuid playerDbId,
              const Uuid sourceSystemDbId,
              const Uuid destinationSystemDbId);
  ~JumpMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getPlayerDbId() const -> Uuid;
  auto tryGetSourceSystemDbId() const -> std::optional<Uuid>;
  auto getSourceSystemDbId() const -> Uuid;
  auto tryGetDestinationSystemDbId() const -> std::optional<Uuid>;
  auto getDestinationSystemDbId() const -> Uuid;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_shipDbId{};
  Uuid m_playerDbId{};
  std::optional<Uuid> m_sourceSystemDbId{};
  std::optional<Uuid> m_destinationSystemDbId{};

  friend auto operator<<(std::ostream &out, const JumpMessage &message) -> std::ostream &;
  friend auto operator>>(std::istream &in, JumpMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const JumpMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, JumpMessage &message) -> std::istream &;

} // namespace bsgalone::core
