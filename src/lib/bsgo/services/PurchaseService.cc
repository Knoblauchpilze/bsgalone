
#include "PurchaseService.hh"

namespace bsgo {

PurchaseService::PurchaseService(const Repositories &repositories)
  : AbstractService("purchase", repositories)
{}

} // namespace bsgo
