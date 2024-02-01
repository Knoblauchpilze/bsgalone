

#pragma once

#include "Uuid.hh"
#include "ValidatableMessage.hh"

namespace bsgo {

class DockMessage : public ValidatableMessage
{
  public:
  DockMessage();
  DockMessage(const Uuid shipDbId, const bool docking);
  ~DockMessage() override = default;

  auto getShipDbId() const -> Uuid;
  bool isDocking() const;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_shipDbId{};
  bool m_docking{false};
};

} // namespace bsgo
