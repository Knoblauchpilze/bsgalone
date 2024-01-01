
#pragma once

#include "SystemMessage.hh"
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

class StatusMessage : public SystemMessage
{
  public:
  StatusMessage(const Uuid &shipDbId, const Uuid &shipEntityId, const JumpState &jumpState);
  StatusMessage(const Uuid &shipDbId,
                const Uuid &shipEntityId,
                const JumpState &jumpState,
                const std::optional<Uuid> &system);
  ~StatusMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getShipEntityId() const -> Uuid;
  auto getJumpState() const -> JumpState;
  auto getJumpSystem() const -> std::optional<Uuid>;

  private:
  Uuid m_shipDbId{};
  Uuid m_shipEntityId{};
  JumpState m_jumpState{};
  std::optional<Uuid> m_system{};
};

} // namespace bsgo
