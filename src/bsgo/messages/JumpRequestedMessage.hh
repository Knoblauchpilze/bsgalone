
#pragma once

#include "NetworkMessage.hh"
#include "Uuid.hh"
#include <optional>

namespace bsgo {

class JumpRequestedMessage : public bsgalone::core::NetworkMessage
{
  public:
  JumpRequestedMessage();
  JumpRequestedMessage(const Uuid shipDbId, const Uuid systemDbId);
  ~JumpRequestedMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getJumpSystem() const -> Uuid;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> bsgalone::core::IMessagePtr override;

  private:
  Uuid m_shipDbId{};
  Uuid m_systemDbId{};
};

} // namespace bsgo
