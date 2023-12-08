
#pragma once

#include "AbstractService.hh"
#include <memory>

namespace bsgo {

class PurchaseService : public AbstractService
{
  public:
  PurchaseService(const Repositories &repositories);
  ~PurchaseService() override = default;
};

using PurchaseServiceShPtr = std::shared_ptr<PurchaseService>;

} // namespace bsgo
