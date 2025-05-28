

#include "HangarMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

HangarMessage::HangarMessage()
  : ValidatableMessage(MessageType::HANGAR)
{}

HangarMessage::HangarMessage(const Uuid shipDbId)
  : ValidatableMessage(MessageType::HANGAR)
  , m_shipDbId(shipDbId)
{}

auto HangarMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto HangarMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);
  core::serialize(out, m_validated);

  core::serialize(out, m_shipDbId);

  return out;
}

bool HangarMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);
  ok &= core::deserialize(in, m_validated);

  ok &= core::deserialize(in, m_shipDbId);

  return ok;
}

auto HangarMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<HangarMessage>(m_shipDbId);
  clone->copyClientIdIfDefined(*this);
  clone->validate(validated());

  return clone;
}

} // namespace bsgo
