
#pragma once

#include "AbstractComponent.hh"
#include "Uuid.hh"
#include <core_utils/TimeUtils.hh>
#include <optional>

namespace bsgo {

struct SlotComponentData
{
  Uuid dbId{};
  bool offensive{};
  float powerCost{};
  std::optional<float> range{};
  utils::Duration reloadTime{};
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

class SlotComponent : public AbstractComponent
{
  public:
  SlotComponent(const ComponentType &type, const SlotComponentData &data);
  ~SlotComponent() override = default;

  void update(const float elapsedSeconds) override;

  auto dbId() const -> Uuid;
  bool isOffensive() const;
  auto powerCost() const -> float;
  auto maybeRange() const -> std::optional<float>;
  auto range() const -> float;
  auto firingState() const noexcept -> FiringState;
  bool canFire() const noexcept;
  bool isReloading() const noexcept;
  auto reloadPercentage() const -> float;
  auto elapsedSinceLastFired() const -> std::optional<utils::Duration>;

  void overrideElapsedSinceLastFired(const std::optional<utils::Duration> &elapsed);
  void setFiringState(const FiringState &firingState);
  void registerFireRequest();
  bool hasFireRequest() const;
  void fire();
  void clearFireRequest();

  private:
  Uuid m_dbId{};
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
