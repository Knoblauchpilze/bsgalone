
#include "DockMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

DockMessage::DockMessage()
  : NetworkMessage(bsgalone::core::MessageType::DOCK)
{}

DockMessage::DockMessage(const Uuid shipDbId, const DockTransition transition)
  : NetworkMessage(bsgalone::core::MessageType::DOCK)
  , m_shipDbId(shipDbId)
  , m_transition(transition)
{}

auto DockMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto DockMessage::getTransition() const -> DockTransition
{
  return m_transition;
}

auto DockMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_shipDbId);
  core::serialize(out, m_transition);

  return out;
}

bool DockMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_shipDbId);
  ok &= core::deserialize(in, m_transition);

  return ok;
}

auto DockMessage::clone() const -> bsgalone::core::IMessagePtr
{
  auto clone = std::make_unique<DockMessage>(m_shipDbId, m_transition);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
