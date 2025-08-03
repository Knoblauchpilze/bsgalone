
#pragma once

#include "NetworkMessage.hh"
#include "PlayerWeaponData.hh"
#include "Uuid.hh"

namespace bsgo {

class PlayerWeaponListMessage : public NetworkMessage
{
  public:
  PlayerWeaponListMessage();
  PlayerWeaponListMessage(const std::vector<PlayerWeaponData> &weaponsData);
  ~PlayerWeaponListMessage() override = default;

  auto getWeaponsData() const -> const std::vector<PlayerWeaponData> &;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  std::vector<PlayerWeaponData> m_weaponsData{};
};

} // namespace bsgo
