
#include "JumpMessage.hh"
#include <core_utils/SerializationUtils.hh>

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

auto JumpMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);

  utils::serialize(out, m_shipDbId);
  utils::serialize(out, m_shipEntityId);
  utils::serialize(out, m_jumpState);
  utils::serialize(out, m_system);

  return out;
}

bool JumpMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);

  ok &= utils::deserialize(in, m_shipDbId);
  ok &= utils::deserialize(in, m_shipEntityId);
  ok &= utils::deserialize(in, m_jumpState);
  ok &= utils::deserialize(in, m_system);

  return ok;
}

} // namespace bsgo
