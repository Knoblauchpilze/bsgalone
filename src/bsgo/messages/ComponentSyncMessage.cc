
#include "ComponentSyncMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

ComponentSyncMessage::ComponentSyncMessage()
  : NetworkMessage(MessageType::COMPONENT_SYNC)
{}

ComponentSyncMessage::ComponentSyncMessage(const Uuid shipDbId, const Uuid systemDbId)
  : NetworkMessage(MessageType::COMPONENT_SYNC)
  , m_shipDbId(shipDbId)
  , m_systemDbId(systemDbId)
{}

auto ComponentSyncMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto ComponentSyncMessage::getSystemDbId() const -> Uuid
{
  return m_systemDbId;
}

auto ComponentSyncMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);

  utils::serialize(out, m_shipDbId);
  utils::serialize(out, m_systemDbId);

  return out;
}

bool ComponentSyncMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);

  ok &= utils::deserialize(in, m_shipDbId);
  ok &= utils::deserialize(in, m_systemDbId);

  return ok;
}

auto ComponentSyncMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<ComponentSyncMessage>(m_shipDbId, m_systemDbId);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
