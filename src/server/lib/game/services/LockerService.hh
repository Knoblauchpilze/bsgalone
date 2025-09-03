
#pragma once

#include "AbstractService.hh"
#include <memory>

namespace bsgo {

struct LockerItemData
{
  Uuid dbId{};
  Item type{};
  Uuid shipDbId{};
};

class LockerService : public AbstractService
{
  public:
  LockerService(const Repositories &repositories);
  ~LockerService() override = default;

  bool tryEquip(const LockerItemData &item) const;
  bool tryUnequip(const LockerItemData &item) const;

  private:
  bool verifySlotAvailability(const Uuid shipDbId, const Item &type) const;
  bool verifyItemBelongsToPlayer(const Uuid playerDbId, const Uuid itemId, const Item &type) const;
  bool verifyItemIsEquiped(const Uuid shipDbId, const Uuid itemId, const Item &type) const;
  bool verifyItemIsNotEquiped(const Uuid itemId, const Item &type) const;
  void tryEquipWeapon(const Uuid shipDbId, const Uuid weaponId) const;
  void tryEquipComputer(const Uuid shipDbId, const Uuid computerId) const;
  void tryUnequipWeapon(const Uuid shipDbId, const Uuid weaponId) const;
  void tryUnequipComputer(const Uuid shipDbId, const Uuid computerId) const;
};

using LockerServiceShPtr = std::shared_ptr<LockerService>;

} // namespace bsgo
