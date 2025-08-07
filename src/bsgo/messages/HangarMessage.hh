

#pragma once

#include "PlayerShipData.hh"
#include "Uuid.hh"
#include "ValidatableMessage.hh"

namespace bsgo {

class HangarMessage : public ValidatableMessage
{
  public:
  HangarMessage();
  HangarMessage(const Uuid shipDbId);
  HangarMessage(const PlayerShipData ship);
  ~HangarMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getShip() const -> PlayerShipData;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  PlayerShipData m_ship{};
};

} // namespace bsgo
