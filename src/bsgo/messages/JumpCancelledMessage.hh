
#pragma once

#include "Uuid.hh"
#include "ValidatableMessage.hh"
#include <optional>

namespace bsgo {

class JumpCancelledMessage : public ValidatableMessage
{
  public:
  JumpCancelledMessage();
  JumpCancelledMessage(const Uuid &shipDbId, const Uuid &shipEntityId);
  ~JumpCancelledMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getShipEntityId() const -> Uuid;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  private:
  Uuid m_shipDbId{};
  Uuid m_shipEntityId{};
};

} // namespace bsgo
