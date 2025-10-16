
#pragma once

#include "AbstractComponent.hh"
#include "Status.hh"
#include <optional>

namespace bsgo {

class StatusComponent : public AbstractComponent
{
  public:
  StatusComponent(const Status &status,
                  const std::optional<chrono::TickDuration> &jumpTime,
                  const std::optional<chrono::TickDuration> &threatJumpTime);
  ~StatusComponent() = default;

  auto status() const -> Status;
  bool isDead() const;
  auto jumpTime() const -> chrono::TickDuration;
  auto threatJumpTime() const -> chrono::TickDuration;
  bool justChanged() const;
  void resetChanged();
  void resetAppearingTime();
  auto getElapsedSinceLastChange() const -> chrono::TickDuration;
  auto tryGetElapsedSinceLastAppearing() const -> std::optional<chrono::TickDuration>;
  auto getCurrentJumpTime() const -> chrono::TickDuration;
  auto getElapsedSinceJumpStarted() const -> chrono::TickDuration;
  auto getRemainingJumpTime() const -> chrono::TickDuration;

  void setStatus(const Status &status);

  void update(const chrono::TickData &data) override;

  private:
  Status m_status;
  std::optional<chrono::TickDuration> m_jumpTime{};
  std::optional<chrono::TickDuration> m_threatJumpTime{};
  bool m_justChanged{false};
  chrono::TickDuration m_elapsedSinceLastChange{};
  std::optional<chrono::TickDuration> m_elapsedSinceAppearing{};
  std::optional<chrono::TickDuration> m_elapsedSinceJumpStarted{};
  std::optional<chrono::TickDuration> m_currentJumpTime{};

  void updateJumpState(const Status &newStatus, const bool forceUpdate);
  void updateAppearingState(const Status &newStatus);
};

using StatusComponentShPtr = std::shared_ptr<StatusComponent>;

} // namespace bsgo
