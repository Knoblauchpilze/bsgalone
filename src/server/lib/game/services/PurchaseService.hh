
#pragma once

#include "AbstractService.hh"
#include <memory>

namespace bsgo {

class PurchaseService : public AbstractService
{
  public:
  PurchaseService(const Repositories &repositories);
  ~PurchaseService() override = default;

  bool tryPurchase(const Uuid playerId, const Uuid itemId, const Item &type) const;

  private:
  bool verifyAffordability(const Uuid playerId, const Uuid itemId, const Item &type) const;
  bool verifyPreconditions(const Uuid playerId, const Uuid itemId, const Item &type) const;
  void tryPurchaseWeapon(const Uuid playerId, const Uuid weaponId) const;
  void tryPurchaseComputer(const Uuid playerId, const Uuid computerId) const;
  void tryPurchaseShip(const Uuid playerId, const Uuid shipId) const;
  void debitResources(const Uuid playerId, const std::unordered_map<Uuid, int> &costs) const;
};

using PurchaseServiceShPtr = std::shared_ptr<PurchaseService>;

} // namespace bsgo
