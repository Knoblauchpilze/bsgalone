

#include "HangarMessage.hh"
#include <core_utils/SerializationUtils.hh>

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

auto HangarMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);

  out << m_shipDbId;
  utils::serialize(out, m_state);

  return out;
}

auto HangarMessage::deserialize(std::istream &in) -> std::istream &
{
  utils::deserialize(in, m_messageType);

  in >> m_shipDbId;
  utils::deserialize(in, m_state);

  return in;
}

} // namespace bsgo
