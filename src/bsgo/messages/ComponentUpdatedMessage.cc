
#include "ComponentUpdatedMessage.hh"

namespace bsgo {

ComponentUpdatedMessage::ComponentUpdatedMessage(const MessageType &type)
  : ComponentUpdatedMessage(type, {}, {})
{}

ComponentUpdatedMessage::ComponentUpdatedMessage(const MessageType &type,
                                                 const Uuid shipDbId,
                                                 const ComponentType component)
  : NetworkMessage(type)
  , m_shipDbId(shipDbId)
  , m_component(component)
{}

auto ComponentUpdatedMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto ComponentUpdatedMessage::getComponentType() const -> ComponentType
{
  return m_component;
}

} // namespace bsgo
