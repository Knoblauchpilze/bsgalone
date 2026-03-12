
#pragma once

#include "NetworkMessage.hh"
#include "Uuid.hh"
#include "WeaponData.hh"

namespace bsgalone::core {

class WeaponListMessage : public NetworkMessage
{
  public:
  WeaponListMessage();
  WeaponListMessage(const std::vector<WeaponData> &weaponsData);
  ~WeaponListMessage() override = default;

  auto getWeaponsData() const -> const std::vector<WeaponData> &;

  auto clone() const -> IMessagePtr override;

  private:
  std::vector<WeaponData> m_weaponsData{};

  friend auto operator<<(std::ostream &out, const WeaponListMessage &message) -> std::ostream &;
  friend auto operator>>(std::istream &in, WeaponListMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const WeaponListMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, WeaponListMessage &message) -> std::istream &;

} // namespace bsgalone::core
