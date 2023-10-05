
#pragma once

#include "IView.hh"
#include "Uuid.hh"
#include <core_utils/CoreObject.hh>
#include <memory>
#include <optional>

namespace bsgo {

class ShipView : public utils::CoreObject, public IView
{
  public:
  ShipView();
  ~ShipView() override = default;

  void update(const float elapsedSeconds) override;

  auto getUuid() const noexcept -> Uuid;

  auto getHealth() const noexcept -> float;
  auto getMaxHealth() const noexcept -> float;
  auto getPower() const noexcept -> float;
  auto getMaxPower() const noexcept -> float;

  bool hasTarget() const noexcept;
  void setTarget(const Uuid &uuid);
  void clearTarget();

  auto getTargetHealth() const noexcept -> float;
  auto getTargetMaxHealth() const noexcept -> float;
  auto getTargetPower() const noexcept -> float;
  auto getTargetMaxPower() const noexcept -> float;

  private:
  std::optional<Uuid> m_target{};
};

using ShipViewShPtr = std::shared_ptr<ShipView>;

} // namespace bsgo
