

#include "HangarMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

HangarMessage::HangarMessage()
  : ValidatableMessage(MessageType::HANGAR)
{}

HangarMessage::HangarMessage(const Uuid &shipDbId)
  : ValidatableMessage(MessageType::HANGAR)
  , m_shipDbId(shipDbId)
{}

auto HangarMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto HangarMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);
  utils::serialize(out, m_validated);

  utils::serialize(out, m_shipDbId);

  return out;
}

bool HangarMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);
  ok &= utils::deserialize(in, m_validated);

  ok &= utils::deserialize(in, m_shipDbId);

  return ok;
}

} // namespace bsgo
