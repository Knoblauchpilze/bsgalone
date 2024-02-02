
#pragma once

#include "Uuid.hh"
#include "ValidatableMessage.hh"
#include <optional>

namespace bsgo {

class JumpCancelledMessage : public ValidatableMessage
{
  public:
  JumpCancelledMessage();
  JumpCancelledMessage(const Uuid shipDbId);
  ~JumpCancelledMessage() override = default;

  auto getShipDbId() const -> Uuid;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_shipDbId{};
};

} // namespace bsgo
