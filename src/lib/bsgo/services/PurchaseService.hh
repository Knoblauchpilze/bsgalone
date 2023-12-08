
#pragma once

#include "AbstractService.hh"
#include <memory>

namespace bsgo {

class PurchaseService : public AbstractService
{
  public:
  PurchaseService(const Repositories &repositories);
  ~PurchaseService() override = default;

  void setPlayerDbId(const Uuid &player);
  bool isReady() const noexcept override;

  bool tryPurchase(const Uuid &id, const Item &type);

  private:
  std::optional<Uuid> m_playerDbId{};

  void checkPlayerDbIdExists() const;
};

using PurchaseServiceShPtr = std::shared_ptr<PurchaseService>;

} // namespace bsgo
