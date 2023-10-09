
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
  TargetView(const CoordinatorShPtr &coordinator);
  ~TargetView() override = default;

  void update(const float elapsedSeconds) override;

  void setTarget(const Uuid &uuid);
  auto getTarget() const -> std::optional<Entity>;
  void clearTarget();

  auto getTargetHealth() const noexcept -> float;
  auto getTargetMaxHealth() const noexcept -> float;
  auto getTargetPower() const noexcept -> float;
  auto getTargetMaxPower() const noexcept -> float;

  private:
  std::optional<Uuid> m_target{};
  CoordinatorShPtr m_coordinator{};
};

using TargetViewShPtr = std::shared_ptr<TargetView>;

} // namespace bsgo
