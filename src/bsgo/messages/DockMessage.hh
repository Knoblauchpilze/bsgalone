

#pragma once

#include "DockTransition.hh"
#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class DockMessage : public bsgalone::core::NetworkMessage
{
  public:
  DockMessage();
  DockMessage(const Uuid shipDbId, const DockTransition transition);
  ~DockMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getTransition() const -> DockTransition;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> bsgalone::core::IMessagePtr override;

  private:
  Uuid m_shipDbId{};
  DockTransition m_transition{DockTransition::UNDOCK};
};

} // namespace bsgo
