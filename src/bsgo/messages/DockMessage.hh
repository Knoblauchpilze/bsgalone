

#pragma once

#include "Uuid.hh"
#include "ValidatableMessage.hh"

namespace bsgo {

class DockMessage : public ValidatableMessage
{
  public:
  DockMessage();
  DockMessage(const Uuid shipDbId, const bool docking, const Uuid systemDbId);
  ~DockMessage() override = default;

  auto getShipDbId() const -> Uuid;
  bool isDocking() const;
  auto getSystemDbId() const -> Uuid;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_shipDbId{};
  bool m_docking{false};
  Uuid m_systemDbId{};
};

} // namespace bsgo
