

#pragma once

#include "Uuid.hh"
#include "ValidatableMessage.hh"

namespace bsgo {

class DockMessage : public ValidatableMessage
{
  public:
  DockMessage();
  DockMessage(const Uuid &shipDbId, const Uuid &shipEntityId, const bool docking);
  ~DockMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getShipEntityId() const -> Uuid;
  bool isDocking() const;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  private:
  Uuid m_shipDbId{};
  Uuid m_shipEntityId{};
  bool m_docking{false};
};

} // namespace bsgo
