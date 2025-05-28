
#include "DockMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

DockMessage::DockMessage()
  : ValidatableMessage(MessageType::DOCK)
{}

DockMessage::DockMessage(const Uuid shipDbId, const bool docking, const Uuid systemDbId)
  : ValidatableMessage(MessageType::DOCK)
  , m_shipDbId(shipDbId)
  , m_docking(docking)
  , m_systemDbId(systemDbId)
{}

auto DockMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

bool DockMessage::isDocking() const
{
  return m_docking;
}

auto DockMessage::getSystemDbId() const -> Uuid
{
  return m_systemDbId;
}

auto DockMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);
  core::serialize(out, m_validated);

  core::serialize(out, m_shipDbId);
  core::serialize(out, m_docking);
  core::serialize(out, m_systemDbId);

  return out;
}

bool DockMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);
  ok &= core::deserialize(in, m_validated);

  ok &= core::deserialize(in, m_shipDbId);
  ok &= core::deserialize(in, m_docking);
  ok &= core::deserialize(in, m_systemDbId);

  return ok;
}

auto DockMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<DockMessage>(m_shipDbId, m_docking, m_systemDbId);
  clone->copyClientIdIfDefined(*this);
  clone->validate(validated());

  return clone;
}

} // namespace bsgo
