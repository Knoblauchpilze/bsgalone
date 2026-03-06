
#pragma once

#include "AbstractService.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <optional>

namespace bsgalone::server {

class ShipService : public AbstractService
{
  public:
  ShipService(const core::Repositories &repositories,
              core::CoordinatorShPtr coordinator,
              core::DatabaseEntityMapper &entityMapper);
  ~ShipService() override = default;

  bool trySelectShip(const core::Uuid shipDbId) const;
  bool tryDock(const core::Uuid shipDbId) const;
  bool tryReturnToOutpost(const core::Uuid shipDbId) const;

  bool accelerateShip(const core::Uuid shipDbId, const Eigen::Vector3f &acceleration) const;

  auto getPlayerDbIdForShip(const core::Uuid shipDbId) -> core::Uuid;
  auto getSystemDbIdForShip(const core::Uuid shipDbId) const -> core::Uuid;

  struct TargetAcquiringData
  {
    core::Uuid sourceDbId{};
    core::EntityKind sourceKind{};

    Eigen::Vector3f position{};

    std::optional<core::Uuid> targetDbIdHint{};
    std::optional<core::EntityKind> targetKindHint{};
  };
  struct AcquiringResult
  {
    bool success{false};
    std::optional<core::EntityKind> targetKind{};
    std::optional<core::Uuid> targetDbId{};
  };
  auto tryAcquireTarget(const TargetAcquiringData &data) const -> AcquiringResult;

  private:
  core::CoordinatorShPtr m_coordinator{};
  core::DatabaseEntityMapper &m_entityMapper;

  void switchActiveShip(core::PlayerShip currentActiveShip, core::PlayerShip newActiveShip) const;
  void switchShipSystem(const core::PlayerShip &currentActiveShip,
                        const core::PlayerShip &newActiveShip) const;

  void updateEntityTarget(core::Entity &entity, const std::optional<core::Uuid> &targetId) const;

  bool isSelfSelection(const TargetAcquiringData &data, const core::Entity &target) const;
};

using ShipServiceShPtr = std::shared_ptr<ShipService>;

} // namespace bsgalone::server
