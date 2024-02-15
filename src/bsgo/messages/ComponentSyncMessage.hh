

#pragma once

#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class ComponentSyncMessage : public NetworkMessage
{
  public:
  ComponentSyncMessage();
  ComponentSyncMessage(const Uuid shipDbId, const Uuid systemDbId);
  ~ComponentSyncMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getSystemDbId() const -> Uuid;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_shipDbId{};
  Uuid m_systemDbId{};
};

} // namespace bsgo
