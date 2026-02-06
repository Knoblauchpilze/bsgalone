
#pragma once

#include "NetworkMessage.hh"
#include "Uuid.hh"
#include <optional>

namespace bsgo {

class JumpCancelledMessage : public NetworkMessage
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
