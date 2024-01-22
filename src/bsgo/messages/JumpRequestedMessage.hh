
#pragma once

#include "Uuid.hh"
#include "ValidatableMessage.hh"
#include <optional>

namespace bsgo {

class JumpRequestedMessage : public ValidatableMessage
{
  public:
  JumpRequestedMessage();
  JumpRequestedMessage(const Uuid &shipDbId, const Uuid &shipEntityId, const Uuid &system);
  ~JumpRequestedMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getShipEntityId() const -> Uuid;
  auto getJumpSystem() const -> Uuid;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  private:
  Uuid m_shipDbId{};
  Uuid m_shipEntityId{};
  Uuid m_system{};
};

} // namespace bsgo
