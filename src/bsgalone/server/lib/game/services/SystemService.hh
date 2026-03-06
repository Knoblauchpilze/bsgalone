
#pragma once

#include "AbstractService.hh"
#include <memory>
#include <optional>

namespace bsgalone::server {

class SystemService : public AbstractService
{
  public:
  SystemService(const core::Repositories &repositories);
  ~SystemService() override = default;

  bool tryDistributeResource(const core::Uuid playerDbId,
                             const core::Uuid resourceDbId,
                             const int amount) const;

  bool disposeOfPlayerShip(const core::Uuid shipDbId, const bool dead) const;

  struct ForcedDockResult
  {
    bool alreadyDocked{false};
  };
  auto sendPlayerBackToOutpost(const core::Uuid &playerDbId) const -> ForcedDockResult;

  struct JumpResult
  {
    bool success{false};
    core::Uuid sourceSystem{};
    core::Uuid destinationSystem{};
  };
  auto tryJump(const core::Uuid shipDbId) const -> JumpResult;

  bool registerAiBehaviorMilestone(const core::Uuid shipDbId, const int targetReached) const;

  auto tryGetSystemDbIdForShip(const core::Uuid shipDbId) const -> std::optional<core::Uuid>;
  auto getSystemDbIdForPlayer(const core::Uuid playerDbId) const -> core::Uuid;
  auto getSystemDbIdForAsteroid(const core::Uuid asteroidDbId) const -> core::Uuid;
  auto getSystemDbIdForOutpost(const core::Uuid outpostDbId) const -> core::Uuid;
  auto getShipDbIdForPlayer(const core::Uuid playerDbId) const -> core::Uuid;

  bool tryMarkAsteroidForRespawn(const core::Uuid asteroidDbId) const;

  private:
  auto findExistingResourceAmount(const core::Uuid playerDbId, const core::Uuid resourceDbId) const
    -> int;
  bool disposeOfAiShip(const core::PlayerShip &playerShip, const bool dead) const;
  bool disposeOfPlayerShip(core::PlayerShip playerShip, const bool dead) const;
};

using SystemServiceShPtr = std::shared_ptr<SystemService>;

} // namespace bsgalone::server
