
#pragma once

#include "IComponent.hh"
#include "Status.hh"
#include <core_utils/TimeUtils.hh>
#include <optional>

namespace bsgo {

class StatusComponent : public IComponent
{
  public:
  StatusComponent(const Status &status,
                  const std::optional<utils::Duration> &jumpTime,
                  const std::optional<utils::Duration> &threatJumpTime);
  ~StatusComponent() = default;

  auto status() const -> Status;
  bool justChanged() const;
  void resetChanged();
  auto getElapsedSinceLastChange() const -> utils::Duration;
  void setStatus(const Status &status);

  void update(const float elapsedSeconds) override;

  private:
  Status m_status;
  std::optional<utils::Duration> m_jumpTime{};
  std::optional<utils::Duration> m_threatJumpTime{};
  bool m_justChanged{false};
  utils::Duration m_elapsedSinceLastChange{};
};

using StatusComponentShPtr = std::shared_ptr<StatusComponent>;

} // namespace bsgo
