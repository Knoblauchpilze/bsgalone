
#include "ComponentUpdatedMessage.hh"

namespace bsgo {

ComponentUpdatedMessage::ComponentUpdatedMessage(const bsgalone::core::MessageType &type)
  : ComponentUpdatedMessage(type, {}, {})
{}

ComponentUpdatedMessage::ComponentUpdatedMessage(const bsgalone::core::MessageType &type,
                                                 const Uuid shipDbId,
                                                 const bsgalone::core::ComponentType component)
  : NetworkMessage(type)
  , m_shipDbId(shipDbId)
  , m_component(component)
{}

auto ComponentUpdatedMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto ComponentUpdatedMessage::getComponentType() const -> bsgalone::core::ComponentType
{
  return m_component;
}

} // namespace bsgo
