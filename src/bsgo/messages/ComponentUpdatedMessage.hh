
#pragma once

#include "ComponentType.hh"
#include "NetworkMessage.hh"

namespace bsgo {

class ComponentUpdatedMessage : public NetworkMessage
{
  public:
  ComponentUpdatedMessage(const MessageType &type);
  ComponentUpdatedMessage(const MessageType &type,
                          const Uuid entityId,
                          const ComponentType component);
  ~ComponentUpdatedMessage() override = default;

  auto getEntityId() const -> Uuid;
  auto getComponentType() const -> ComponentType;

  protected:
  Uuid m_entityId{};
  ComponentType m_component{};
};

} // namespace bsgo
