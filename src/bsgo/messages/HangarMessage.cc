

#include "HangarMessage.hh"

namespace bsgo {

HangarMessage::HangarMessage(const Uuid &shipDbId, const ShipSwitchRequestState &state)
  : AbstractMessage(MessageType::HANGAR)
  , m_shipDbId(shipDbId)
  , m_state(state)
{}

auto HangarMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto HangarMessage::getRequestState() const -> ShipSwitchRequestState
{
  return m_state;
}

} // namespace bsgo
