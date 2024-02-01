
#pragma once

#include "ComponentType.hh"
#include "NetworkMessage.hh"

namespace bsgo {

class ComponentUpdatedMessage : public NetworkMessage
{
  public:
  ComponentUpdatedMessage(const MessageType &type);
  ComponentUpdatedMessage(const MessageType &type,
                          const Uuid shipDbId,
                          const ComponentType component);
  ~ComponentUpdatedMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getComponentType() const -> ComponentType;

  protected:
  Uuid m_shipDbId{};
  ComponentType m_component{};
};

} // namespace bsgo
