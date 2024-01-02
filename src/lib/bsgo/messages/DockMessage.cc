
#include "DockMessage.hh"

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

} // namespace bsgo
