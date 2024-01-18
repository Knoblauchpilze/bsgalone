

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

  utils::serialize(out, m_shipDbId);
  utils::serialize(out, m_state);

  return out;
}

bool HangarMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);

  ok &= utils::deserialize(in, m_shipDbId);
  ok &= utils::deserialize(in, m_state);

  return ok;
}

} // namespace bsgo
