
#include "JumpMessage.hh"

namespace bsgo {

JumpMessage::JumpMessage(const Uuid &shipDbId, const Uuid &shipEntityId, const JumpState &jumpState)
  : JumpMessage(shipDbId, shipEntityId, jumpState, {})
{}

JumpMessage::JumpMessage(const Uuid &shipDbId,
                         const Uuid &shipEntityId,
                         const JumpState &jumpState,
                         const std::optional<Uuid> &system)
  : AbstractMessage(MessageType::JUMP)
  , m_shipDbId(shipDbId)
  , m_shipEntityId(shipEntityId)
  , m_jumpState(jumpState)
  , m_system(system)
{}

auto JumpMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto JumpMessage::getShipEntityId() const -> Uuid
{
  return m_shipEntityId;
}

auto JumpMessage::getJumpState() const -> JumpState
{
  return m_jumpState;
}

auto JumpMessage::getJumpSystem() const -> std::optional<Uuid>
{
  return m_system;
}

} // namespace bsgo
