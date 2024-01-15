
#pragma once

#include "AbstractMessage.hh"
#include "Uuid.hh"
#include <optional>

namespace bsgo {

enum class JumpState
{
  STARTED,
  CANCELLED,
  RUNNING,
  COMPLETED
};

class JumpMessage : public AbstractMessage
{
  public:
  JumpMessage(const Uuid &shipDbId, const Uuid &shipEntityId, const JumpState &jumpState);
  JumpMessage(const Uuid &shipDbId,
              const Uuid &shipEntityId,
              const JumpState &jumpState,
              const std::optional<Uuid> &system);
  ~JumpMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getShipEntityId() const -> Uuid;
  auto getJumpState() const -> JumpState;
  auto getJumpSystem() const -> std::optional<Uuid>;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  auto deserialize(std::istream &in) -> std::istream & override;

  private:
  Uuid m_shipDbId{};
  Uuid m_shipEntityId{};
  JumpState m_jumpState{};
  std::optional<Uuid> m_system{};
};

} // namespace bsgo
