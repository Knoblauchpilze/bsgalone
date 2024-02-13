
#include "DockMessage.hh"
#include <core_utils/SerializationUtils.hh>

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
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);
  utils::serialize(out, m_validated);

  utils::serialize(out, m_shipDbId);
  utils::serialize(out, m_docking);
  utils::serialize(out, m_systemDbId);

  return out;
}

bool DockMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);
  ok &= utils::deserialize(in, m_validated);

  ok &= utils::deserialize(in, m_shipDbId);
  ok &= utils::deserialize(in, m_docking);
  ok &= utils::deserialize(in, m_systemDbId);

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
