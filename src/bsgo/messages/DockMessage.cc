
#include "DockMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

DockMessage::DockMessage()
  : ValidatableMessage(MessageType::DOCK)
{}

DockMessage::DockMessage(const Uuid &shipDbId, const Uuid &shipEntityId, const bool docking)
  : ValidatableMessage(MessageType::DOCK)
  , m_shipDbId(shipDbId)
  , m_shipEntityId(shipEntityId)
  , m_docking(docking)
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

auto DockMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);
  utils::serialize(out, m_validated);

  utils::serialize(out, m_shipDbId);
  utils::serialize(out, m_shipEntityId);
  utils::serialize(out, m_docking);

  return out;
}

bool DockMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);
  ok &= utils::deserialize(in, m_validated);

  ok &= utils::deserialize(in, m_shipDbId);
  ok &= utils::deserialize(in, m_shipEntityId);
  ok &= utils::deserialize(in, m_docking);

  return ok;
}

} // namespace bsgo
