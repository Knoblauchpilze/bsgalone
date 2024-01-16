
#include "DockMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

DockMessage::DockMessage(const Uuid &shipDbId,
                         const Uuid &shipEntityId,
                         const bool docking,
                         const DockState &state)
  : AbstractMessage(MessageType::DOCK)
  , m_shipDbId(shipDbId)
  , m_shipEntityId(shipEntityId)
  , m_docking(docking)
  , m_state(state)
{}

auto DockMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto DockMessage::getShipEntityId() const -> Uuid
{
  return m_shipEntityId;
}

bool DockMessage::isDocking() const
{
  return m_docking;
}

auto DockMessage::getDockState() const -> DockState
{
  return m_state;
}

auto DockMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);

  utils::serialize(out, m_shipDbId);
  utils::serialize(out, m_shipEntityId);
  utils::serialize(out, m_docking);
  utils::serialize(out, m_state);

  return out;
}

auto DockMessage::deserialize(std::istream &in) -> std::istream &
{
  utils::deserialize(in, m_messageType);

  utils::deserialize(in, m_shipDbId);
  utils::deserialize(in, m_shipEntityId);
  utils::deserialize(in, m_docking);
  utils::deserialize(in, m_state);

  return in;
}

} // namespace bsgo
