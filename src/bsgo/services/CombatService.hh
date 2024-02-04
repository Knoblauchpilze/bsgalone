
#pragma once

#include "AbstractService.hh"
#include <memory>

namespace bsgo {

class CombatService : public AbstractService
{
  public:
  CombatService(const Repositories &repositories);
  ~CombatService() override = default;

  bool tryDistributeResource(const Uuid playerDbId,
                             const Uuid resourceDbId,
                             const float amount) const;

  private:
  auto findExistingResourceAmount(const Uuid playerDbId, const Uuid resourceDbId) const -> float;
};

using CombatServicePtr = std::unique_ptr<CombatService>;

} // namespace bsgo
