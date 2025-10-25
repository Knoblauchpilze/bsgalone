
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

  bool tryDistributeResource(const Uuid playerDbId, const Uuid resourceDbId, const int amount) const;

  bool disposeOfDeadShip(const Uuid shipDbId) const;

  struct ForcedDockResult
  {
    bool alreadyDocked{false};
  };
  auto sendPlayerBackToOutpost(const Uuid &playerDbId) const -> ForcedDockResult;

  struct JumpResult
  {
    bool success{false};
    Uuid sourceSystem{};
    Uuid destinationSystem{};
  };
  auto tryJump(const Uuid shipDbId) const -> JumpResult;

  auto tryGetSystemDbIdForShip(const Uuid shipDbId) const -> std::optional<Uuid>;
  auto getSystemDbIdForAsteroid(const Uuid asteroidDbId) const -> Uuid;
  auto getSystemDbIdForOutpost(const Uuid outpostDbId) const -> Uuid;
  auto getShipDbIdForPlayer(const Uuid playerDbId) const -> Uuid;

  bool tryMarkAsteroidForRespawn(const Uuid asteroidDbId) const;

  private:
  auto findExistingResourceAmount(const Uuid playerDbId, const Uuid resourceDbId) const -> int;
  bool disposeOfDeadAiShip(const PlayerShip &playerShip) const;
  bool disposeOfDeadPlayerShip(PlayerShip playerShip) const;
};

using SystemServiceShPtr = std::shared_ptr<SystemService>;

} // namespace bsgo
