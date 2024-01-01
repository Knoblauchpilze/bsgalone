
#pragma once

#include "AbstractService.hh"
#include <memory>

namespace bsgo {

class PurchaseService : public AbstractService
{
  public:
  PurchaseService(const Repositories &repositories, const CoordinatorShPtr &coordinator);
  ~PurchaseService() override = default;

  void setPlayerDbId(const Uuid &player);
  bool isReady() const noexcept override;

  bool tryPurchase(const Uuid &id, const Item &type) const;

  private:
  std::optional<Uuid> m_playerDbId{};

  void checkPlayerDbIdExists() const;
  bool verifyAffordability(const Uuid &id, const Item &type) const;
  bool verifyPreconditions(const Uuid &id, const Item &type) const;
  void tryPurchaseWeapon(const Uuid &id) const;
  void tryPurchaseComputer(const Uuid &id) const;
  void tryPurchaseShip(const Uuid &id) const;
  void debitResources(const std::unordered_map<Uuid, float> &costs) const;
};

using PurchaseServiceShPtr = std::shared_ptr<PurchaseService>;

} // namespace bsgo
