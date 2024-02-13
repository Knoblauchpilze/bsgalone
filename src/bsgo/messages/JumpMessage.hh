
#pragma once

#include "Uuid.hh"
#include "ValidatableMessage.hh"
#include <optional>

namespace bsgo {

class JumpMessage : public ValidatableMessage
{
  public:
  JumpMessage();
  JumpMessage(const Uuid shipDbId, const Uuid playerDbId);
  ~JumpMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getPlayerDbId() const -> Uuid;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_shipDbId{};
  Uuid m_playerDbId{};
};

} // namespace bsgo
