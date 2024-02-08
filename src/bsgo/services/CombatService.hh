
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

  bool trySendPlayerShipBackToOutpost(const Uuid shipDbId) const;

  private:
  auto findExistingResourceAmount(const Uuid playerDbId, const Uuid resourceDbId) const -> float;
};

using CombatServiceShPtr = std::shared_ptr<CombatService>;

} // namespace bsgo
