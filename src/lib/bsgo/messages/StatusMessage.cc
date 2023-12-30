
#include "StatusMessage.hh"

namespace bsgo {

StatusMessage::StatusMessage(const Uuid &shipDbId, const JumpState &jumpState)
  : StatusMessage(shipDbId, jumpState, {})
{}

StatusMessage::StatusMessage(const Uuid &shipDbId,
                             const JumpState &jumpState,
                             const std::optional<Uuid> &system)
  : SystemMessage(SystemType::STATUS)
  , m_shipDbId(shipDbId)
  , m_jumpState(jumpState)
  , m_system(system)
{}

auto StatusMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto StatusMessage::getJumpState() const -> JumpState
{
  return m_jumpState;
}

auto StatusMessage::getJumpSystem() const -> std::optional<Uuid>
{
  return m_system;
}

} // namespace bsgo
