
#pragma once

#include "Uuid.hh"
#include "ValidatableMessage.hh"
#include <optional>

namespace bsgo {

class JumpRequestedMessage : public ValidatableMessage
{
  public:
  JumpRequestedMessage();
  JumpRequestedMessage(const Uuid shipDbId, const Uuid systemDbId);
  ~JumpRequestedMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getJumpSystem() const -> Uuid;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_shipDbId{};
  Uuid m_systemDbId{};
};

} // namespace bsgo
