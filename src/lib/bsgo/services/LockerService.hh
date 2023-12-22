
#pragma once

#include "AbstractService.hh"
#include <memory>

namespace bsgo {

class LockerService : public AbstractService
{
  public:
  LockerService(const Repositories &repositories);
  ~LockerService() override = default;

  void setPlayerDbId(const Uuid &player);
  void setPlayerShipDbId(const Uuid &ship);
  bool isReady() const noexcept override;

  bool tryEquip(const Uuid &id, const Item &type) const;
  bool tryUnequip(const Uuid &id, const Item &type) const;

  private:
  std::optional<Uuid> m_playerDbId{};
  std::optional<Uuid> m_playerShipDbId{};

  void checkPlayerDbIdExists() const;
  void checkPlayerShipDbIdExists() const;

  bool verifySlotAvailability(const Item &type) const;
  bool verifyItemBelongsToPlayer(const Uuid &item, const Item &type) const;
  bool verifyItemIsEquiped(const Uuid &item, const Item &type) const;
  void tryEquipWeapon(const Uuid &id) const;
  void tryEquipComputer(const Uuid &id) const;
  void tryUnequipWeapon(const Uuid &weapon) const;
  void tryUnequipComputer(const Uuid &computer) const;
};

using LockerServiceShPtr = std::shared_ptr<LockerService>;

} // namespace bsgo
