
#pragma once

#include "IComponent.hh"
#include <core_utils/TimeUtils.hh>
#include <optional>

namespace bsgo {

struct SlotComponentData
{
  bool offensive;
  float powerCost;
  std::optional<float> range;
  utils::Duration reloadTime;
};

enum class FiringState
{
  READY,
  DISABLED,
  INVALID_TARGET,
  OUT_OF_RANGE,
  OUT_OF_POWER,
  RELOADING
};

class SlotComponent : public IComponent
{
  public:
  SlotComponent(const std::string &name, const SlotComponentData &data);
  ~SlotComponent() override = default;

  void update(const float elapsedSeconds) override;

  bool isOffensive() const;
  auto powerCost() const -> float;
  auto maybeRange() const -> std::optional<float>;
  auto range() const -> float;
  auto firingState() const noexcept -> FiringState;
  bool canFire() const noexcept;
  bool isReloading() const noexcept;
  auto reloadPercentage() const -> float;

  void setFiringState(const FiringState &firingState);
  void registerFireRequest();
  bool hasFireRequest() const;
  void fire();
  void clearFireRequest();

  private:
  bool m_offensive;
  float m_powerCost;
  std::optional<float> m_range;
  utils::Duration m_reloadTime;

  bool m_fireRequest{false};
  FiringState m_firingState{FiringState::READY};
  std::optional<utils::Duration> m_elapsedSinceLastFired{};

  void handleReload(const float elapsedSeconds);
};

} // namespace bsgo
