
#pragma once

#include "AbstractComponent.hh"
#include "Status.hh"
#include "TimeUtils.hh"
#include <optional>

namespace bsgo {

class StatusComponent : public AbstractComponent
{
  public:
  StatusComponent(const Status &status,
                  const std::optional<TickDuration> &jumpTime,
                  const std::optional<TickDuration> &threatJumpTime);
  ~StatusComponent() = default;

  auto status() const -> Status;
  bool isDead() const;
  auto jumpTime() const -> core::Duration;
  auto threatJumpTime() const -> core::Duration;
  bool justChanged() const;
  void resetChanged();
  void resetAppearingTime();
  auto getElapsedSinceLastChange() const -> core::Duration;
  auto tryGetElapsedSinceLastAppearing() const -> std::optional<core::Duration>;
  auto getCurrentJumpTime() const -> TickDuration;
  auto getElapsedSinceJumpStarted() const -> TickDuration;
  auto getRemainingJumpTime() const -> TickDuration;

  void setStatus(const Status &status);

  void update(const TickData &data) override;

  private:
  Status m_status;
  std::optional<TickDuration> m_jumpTime{};
  std::optional<TickDuration> m_threatJumpTime{};
  bool m_justChanged{false};
  TickDuration m_elapsedSinceLastChange{};
  std::optional<TickDuration> m_elapsedSinceAppearing{};
  std::optional<TickDuration> m_elapsedSinceJumpStarted{};
  std::optional<TickDuration> m_currentJumpTime{};

  void updateJumpState(const Status &newStatus, const bool forceUpdate);
  void updateAppearingState(const Status &newStatus);
};

using StatusComponentShPtr = std::shared_ptr<StatusComponent>;

} // namespace bsgo
