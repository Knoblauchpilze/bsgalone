
#include "StatusMessage.hh"

namespace bsgo {

StatusMessage::StatusMessage(const Uuid &shipDbId, const JumpState &jumpState)
  : SystemMessage(SystemType::STATUS)
  , m_shipDbId(shipDbId)
  , m_jumpState(jumpState)
{}

auto StatusMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto StatusMessage::getJumpState() const -> JumpState
{
  return m_jumpState;
}

} // namespace bsgo
