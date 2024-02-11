
#pragma once

#include "AbstractService.hh"
#include <memory>
#include <optional>

namespace bsgo {

class SystemService : public AbstractService
{
  public:
  SystemService(const Repositories &repositories);
  ~SystemService() override = default;

  bool tryDistributeResource(const Uuid playerDbId,
                             const Uuid resourceDbId,
                             const float amount) const;

  bool trySendPlayerShipBackToOutpost(const Uuid shipDbId) const;
  void trySendPlayerBackToOutpost(const Uuid &playerDbId) const;

  auto getSystemDbIdForAsteroid(const Uuid asteroidDbId) const -> Uuid;
  auto tryGetSystemDbIdForShip(const Uuid shipDbId) const -> std::optional<Uuid>;
  auto getShipDbIdForPlayer(const Uuid playerDbId) const -> Uuid;

  private:
  auto findExistingResourceAmount(const Uuid playerDbId, const Uuid resourceDbId) const -> float;
};

using SystemServiceShPtr = std::shared_ptr<SystemService>;

} // namespace bsgo
