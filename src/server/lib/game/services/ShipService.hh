
#pragma once

#include "AbstractService.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <optional>

namespace bsgo {

class ShipService : public AbstractService
{
  public:
  ShipService(const Repositories &repositories,
              CoordinatorShPtr coordinator,
              DatabaseEntityMapper &entityMapper);
  ~ShipService() override = default;

  bool trySelectShip(const Uuid shipDbId) const;
  bool tryDock(const Uuid shipDbId) const;
  bool tryReturnToOutpost(const Uuid shipDbId) const;

  bool accelerateShip(const Uuid shipDbId, const Eigen::Vector3f &acceleration) const;

  auto getPlayerDbIdForShip(const Uuid shipDbId) -> Uuid;
  auto getSystemDbIdForShip(const Uuid shipDbId) const -> Uuid;

  struct TargetAcquiringData
  {
    Uuid sourceDbId{};
    bsgalone::core::EntityKind sourceKind{};

    Eigen::Vector3f position{};

    std::optional<Uuid> targetDbIdHint{};
    std::optional<bsgalone::core::EntityKind> targetKindHint{};
  };
  struct AcquiringResult
  {
    bool success{false};
    std::optional<bsgalone::core::EntityKind> targetKind{};
    std::optional<Uuid> targetDbId{};
  };
  auto tryAcquireTarget(const TargetAcquiringData &data) const -> AcquiringResult;

  private:
  CoordinatorShPtr m_coordinator{};
  DatabaseEntityMapper &m_entityMapper;

  void switchActiveShip(PlayerShip currentActiveShip, PlayerShip newActiveShip) const;
  void switchShipSystem(const PlayerShip &currentActiveShip, const PlayerShip &newActiveShip) const;

  void updateEntityTarget(Entity &entity, const std::optional<Uuid> &targetId) const;

  bool isSelfSelection(const TargetAcquiringData &data, const Entity &target) const;
};

using ShipServiceShPtr = std::shared_ptr<ShipService>;

} // namespace bsgo
