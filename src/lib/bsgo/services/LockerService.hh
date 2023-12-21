
#pragma once

#include "AbstractService.hh"
#include <memory>

namespace bsgo {

class LockerService : public AbstractService
{
  public:
  LockerService(const Repositories &repositories);
  ~LockerService() override = default;

  void setPlayerShipDbId(const Uuid &ship);
  bool isReady() const noexcept override;

  bool tryEquip(const Uuid &id, const Item &type) const;
  bool tryUnequip(const Uuid &id, const Item &type) const;

  private:
  std::optional<Uuid> m_playerShipDbId{};

  void checkPlayerShipDbIdExists() const;

  bool verifySlotAvailability(const Item &type) const;
  bool verifyItemIsEquiped(const Uuid &item, const Item &type) const;
  auto determineNextSlot(const Item &type) const -> Uuid;
  void tryEquipWeapon(const Uuid &id, const Uuid slot) const;
  void tryUnequipWeapon(const Uuid &weapon) const;
  void tryUnequipComputer(const Uuid &computer) const;
};

using LockerServiceShPtr = std::shared_ptr<LockerService>;

} // namespace bsgo
