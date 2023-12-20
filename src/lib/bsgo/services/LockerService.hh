
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

  private:
  std::optional<Uuid> m_playerDbId{};
  std::optional<Uuid> m_playerShipDbId{};

  void checkPlayerDbIdExists() const;
  void checkPlayerShipDbIdExists() const;

  bool verifySlotAvailability(const Item &type) const;
  auto determineNextSlot(const Item &type) const -> Uuid;
  void tryEquipWeapon(const Uuid &id, const Uuid slot) const;
};

using LockerServiceShPtr = std::shared_ptr<LockerService>;

} // namespace bsgo
