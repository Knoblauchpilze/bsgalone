
#pragma once

#include "ComponentType.hh"
#include "NetworkMessage.hh"

namespace bsgo {

class ComponentUpdatedMessage : public bsgalone::core::NetworkMessage
{
  public:
  ComponentUpdatedMessage(const bsgalone::core::MessageType &type);
  ComponentUpdatedMessage(const bsgalone::core::MessageType &type,
                          const Uuid shipDbId,
                          const bsgalone::core::ComponentType component);
  ~ComponentUpdatedMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getComponentType() const -> bsgalone::core::ComponentType;

  protected:
  Uuid m_shipDbId{};
  bsgalone::core::ComponentType m_component{};
};

} // namespace bsgo
