
#pragma once

#include "AbstractService.hh"
#include <memory>

namespace bsgalone::server {

class PurchaseService : public AbstractService
{
  public:
  PurchaseService(const core::Repositories &repositories);
  ~PurchaseService() override = default;

  bool tryPurchase(const core::Uuid playerId, const core::Uuid itemId, const core::Item &type) const;

  private:
  bool verifyAffordability(const core::Uuid playerId,
                           const core::Uuid itemId,
                           const core::Item &type) const;
  bool verifyPreconditions(const core::Uuid playerId,
                           const core::Uuid itemId,
                           const core::Item &type) const;
  void tryPurchaseWeapon(const core::Uuid playerId, const core::Uuid weaponId) const;
  void tryPurchaseComputer(const core::Uuid playerId, const core::Uuid computerId) const;
  void tryPurchaseShip(const core::Uuid playerId, const core::Uuid shipId) const;
  void debitResources(const core::Uuid playerId,
                      const std::unordered_map<core::Uuid, int> &costs) const;
};

using PurchaseServiceShPtr = std::shared_ptr<PurchaseService>;

} // namespace bsgalone::server
