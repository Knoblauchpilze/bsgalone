
#pragma once

#include "Coordinator.hh"
#include "Entity.hh"
#include "IView.hh"
#include "Uuid.hh"
#include <core_utils/CoreObject.hh>
#include <memory>
#include <optional>

namespace bsgo {

class TargetView : public utils::CoreObject, public IView
{
  public:
  TargetView(const Uuid &playerShipId, const CoordinatorShPtr &coordinator);
  ~TargetView() override = default;

  void update(const float elapsedSeconds) override;

  void setTarget(const Uuid &uuid);
  auto getTarget() const -> std::optional<Entity>;
  auto distanceToTarget() const -> float;
  void clearTarget();

  private:
  Uuid m_playerShipId;
  std::optional<Uuid> m_target{};
  CoordinatorShPtr m_coordinator{};

  auto getPlayerShipPosition() const -> Eigen::Vector3f;
};

using TargetViewShPtr = std::shared_ptr<TargetView>;

} // namespace bsgo
