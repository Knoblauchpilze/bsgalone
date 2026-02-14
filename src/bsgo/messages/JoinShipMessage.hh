
#pragma once

#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class JoinShipMessage : public bsgalone::core::NetworkMessage
{
  public:
  JoinShipMessage();
  JoinShipMessage(const Uuid playerDbId, const Uuid shipDbId);
  ~JoinShipMessage() override = default;

  auto getPlayerDbId() const -> Uuid;
  auto getShipDbId() const -> Uuid;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> bsgalone::core::IMessagePtr override;

  private:
  Uuid m_playerDbId{};
  Uuid m_shipDbId{};
};

} // namespace bsgo
