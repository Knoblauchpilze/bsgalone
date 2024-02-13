
#pragma once

#include "NetworkMessage.hh"
#include "Uuid.hh"
#include <optional>

namespace bsgo {

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

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_shipDbId{};
  Uuid m_playerDbId{};
  std::optional<Uuid> m_sourceSystemDbId{};
  std::optional<Uuid> m_destinationSystemDbId{};
};

} // namespace bsgo
