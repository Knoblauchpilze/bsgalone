
#pragma once

#include "Uuid.hh"
#include "ValidatableMessage.hh"

namespace bsgo {

class JoinShipMessage : public ValidatableMessage
{
  public:
  JoinShipMessage();
  JoinShipMessage(const Uuid playerDbId, const Uuid shipDbId);
  ~JoinShipMessage() override = default;

  auto getPlayerDbId() const -> Uuid;
  auto getShipDbId() const -> Uuid;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_playerDbId{};
  Uuid m_shipDbId{};
};

} // namespace bsgo
