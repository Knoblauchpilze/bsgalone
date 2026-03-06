
#pragma once

#include "AbstractService.hh"
#include <memory>

namespace bsgalone::server {

struct LockerItemData
{
  core::Uuid dbId{};
  core::Item type{};
  core::Uuid shipDbId{};
};

class LockerService : public AbstractService
{
  public:
  LockerService(const core::Repositories &repositories);
  ~LockerService() override = default;

  bool tryEquip(const LockerItemData &item) const;
  bool tryUnequip(const LockerItemData &item) const;

  private:
  bool verifySlotAvailability(const core::Uuid shipDbId, const core::Item &type) const;
  bool verifyItemBelongsToPlayer(const core::Uuid playerDbId,
                                 const core::Uuid itemId,
                                 const core::Item &type) const;
  bool verifyItemIsEquiped(const core::Uuid shipDbId,
                           const core::Uuid itemId,
                           const core::Item &type) const;
  bool verifyItemIsNotEquiped(const core::Uuid itemId, const core::Item &type) const;
  void tryEquipWeapon(const core::Uuid shipDbId, const core::Uuid weaponId) const;
  void tryEquipComputer(const core::Uuid shipDbId, const core::Uuid computerId) const;
  void tryUnequipWeapon(const core::Uuid shipDbId, const core::Uuid weaponId) const;
  void tryUnequipComputer(const core::Uuid shipDbId, const core::Uuid computerId) const;
};

using LockerServiceShPtr = std::shared_ptr<LockerService>;

} // namespace bsgalone::server
