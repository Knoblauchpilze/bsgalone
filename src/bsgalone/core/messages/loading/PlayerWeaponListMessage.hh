
#pragma once

#include "NetworkMessage.hh"
#include "PlayerWeaponData.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class PlayerWeaponListMessage : public NetworkMessage
{
  public:
  PlayerWeaponListMessage();
  PlayerWeaponListMessage(const std::vector<PlayerWeaponData> &weaponsData);
  ~PlayerWeaponListMessage() override = default;

  auto getWeaponsData() const -> const std::vector<PlayerWeaponData> &;

  auto clone() const -> IMessagePtr override;

  private:
  std::vector<PlayerWeaponData> m_weaponsData{};

  friend auto operator<<(std::ostream &out, const PlayerWeaponListMessage &message)
    -> std::ostream &;
  friend auto operator>>(std::istream &in, PlayerWeaponListMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const PlayerWeaponListMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, PlayerWeaponListMessage &message) -> std::istream &;

} // namespace bsgalone::core
