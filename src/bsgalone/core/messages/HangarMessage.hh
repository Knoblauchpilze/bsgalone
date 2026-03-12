

#pragma once

#include "NetworkMessage.hh"
#include "PlayerShipData.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class HangarMessage : public NetworkMessage
{
  public:
  HangarMessage();
  HangarMessage(const Uuid shipDbId);
  HangarMessage(const PlayerShipData ship);
  ~HangarMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getShip() const -> PlayerShipData;

  auto clone() const -> IMessagePtr override;

  private:
  PlayerShipData m_ship{};

  friend auto operator<<(std::ostream &out, const HangarMessage &message) -> std::ostream &;
  friend auto operator>>(std::istream &in, HangarMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const HangarMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, HangarMessage &message) -> std::istream &;

} // namespace bsgalone::core
