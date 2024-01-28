
#include "ComponentUpdatedMessage.hh"

namespace bsgo {

ComponentUpdatedMessage::ComponentUpdatedMessage(const MessageType &type)
  : ComponentUpdatedMessage(type, {}, {})
{}

ComponentUpdatedMessage::ComponentUpdatedMessage(const MessageType &type,
                                                 const Uuid entityId,
                                                 const ComponentType component)
  : NetworkMessage(type)
  , m_entityId(entityId)
  , m_component(component)
{}

auto ComponentUpdatedMessage::getEntityId() const -> Uuid
{
  return m_entityId;
}

auto ComponentUpdatedMessage::getComponentType() const -> ComponentType
{
  return m_component;
}

} // namespace bsgo
