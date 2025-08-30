

#pragma once

#include "DockTransition.hh"
#include "Uuid.hh"
#include "ValidatableMessage.hh"

namespace bsgo {

class DockMessage : public ValidatableMessage
{
  public:
  DockMessage();
  DockMessage(const Uuid shipDbId, const Uuid systemDbId, const DockTransition transition);
  ~DockMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getSystemDbId() const -> Uuid;
  auto getTransition() const -> DockTransition;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_shipDbId{};
  Uuid m_systemDbId{};
  DockTransition m_transition{DockTransition::UNDOCK};
};

} // namespace bsgo
