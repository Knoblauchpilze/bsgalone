
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
                  const std::optional<core::Duration> &jumpTime,
                  const std::optional<core::Duration> &threatJumpTime);
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
  auto tryGetCurrentJumpTime() const -> core::Duration;
  auto tryGetElapsedSinceJumpStarted() const -> core::Duration;
  auto tryGetRemainingJumpTime() const -> core::Duration;

  void setStatus(const Status &status);

  void update(const TickData &data) override;

  private:
  Status m_status;
  std::optional<core::Duration> m_jumpTime{};
  std::optional<core::Duration> m_threatJumpTime{};
  bool m_justChanged{false};
  core::Duration m_elapsedSinceLastChange{};
  std::optional<core::Duration> m_elapsedSinceAppearing{};
  std::optional<core::Duration> m_elapsedSinceJumpStarted{};
  std::optional<core::Duration> m_currentJumpTime{};

  void updateJumpState(const Status &newStatus, const bool forceUpdate);
  void updateAppearingState(const Status &newStatus);
};

using StatusComponentShPtr = std::shared_ptr<StatusComponent>;

} // namespace bsgo
