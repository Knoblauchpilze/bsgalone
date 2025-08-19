
#pragma once

#include "NetworkMessage.hh"
#include "Uuid.hh"
#include "WeaponData.hh"

namespace bsgo {

class WeaponListMessage : public NetworkMessage
{
  public:
  WeaponListMessage();
  WeaponListMessage(const std::vector<WeaponData> &weaponsData);
  ~WeaponListMessage() override = default;

  auto getWeaponsData() const -> const std::vector<WeaponData> &;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  std::vector<WeaponData> m_weaponsData{};
};

} // namespace bsgo
