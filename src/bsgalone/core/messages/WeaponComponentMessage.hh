
#pragma once

#include "ComponentUpdatedMessage.hh"
#include "WeaponSlotComponent.hh"

namespace bsgalone::core {

class WeaponComponentMessage : public ComponentUpdatedMessage
{
  public:
  WeaponComponentMessage();
  WeaponComponentMessage(const Uuid shipDbId, const Uuid weaponDbId, const bool active);
  ~WeaponComponentMessage() override = default;

  auto getWeaponDbId() const -> Uuid;
  bool isActive() const;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_weaponDbId{};
  bool m_active{};

  friend auto operator<<(std::ostream &out, const WeaponComponentMessage &message)
    -> std::ostream &;
  friend auto operator>>(std::istream &in, WeaponComponentMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const WeaponComponentMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, WeaponComponentMessage &message) -> std::istream &;

} // namespace bsgalone::core
