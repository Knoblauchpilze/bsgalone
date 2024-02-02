
#pragma once

#include "ComponentUpdatedMessage.hh"
#include "WeaponSlotComponent.hh"

namespace bsgo {

class WeaponComponentMessage : public ComponentUpdatedMessage
{
  public:
  WeaponComponentMessage();
  WeaponComponentMessage(const Uuid shipDbId, const Uuid weaponDbId, const bool active);
  ~WeaponComponentMessage() override = default;

  auto getWeaponDbId() const -> Uuid;
  bool isActive() const;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_weaponDbId{};
  bool m_active{};
};

} // namespace bsgo
