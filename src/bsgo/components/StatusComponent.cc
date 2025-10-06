
#include "StatusComponent.hh"

namespace bsgo {

StatusComponent::StatusComponent(const Status &status,
                                 const std::optional<core::Duration> &jumpTime,
                                 const std::optional<core::Duration> &threatJumpTime)
  : AbstractComponent(ComponentType::STATUS)
  , m_status(status)
  , m_jumpTime(jumpTime)
  , m_threatJumpTime(threatJumpTime)
{
  updateJumpState(m_status, true);
  updateAppearingState(m_status);
}

auto StatusComponent::status() const -> Status
{
  return m_status;
}

bool StatusComponent::isDead() const
{
  return Status::DEAD == m_status;
}

auto StatusComponent::jumpTime() const -> core::Duration
{
  if (!m_jumpTime)
  {
    error("Failed to get jump time", "No such value");
  }
  return *m_jumpTime;
}

auto StatusComponent::threatJumpTime() const -> core::Duration
{
  if (!m_threatJumpTime)
  {
    error("Failed to get threat jump time", "No such value");
  }
  return *m_threatJumpTime;
}

bool StatusComponent::justChanged() const
{
  return m_justChanged;
}

void StatusComponent::resetChanged()
{
  m_justChanged = false;
}

void StatusComponent::resetAppearingTime()
{
  m_elapsedSinceAppearing = core::Duration{0};
}

auto StatusComponent::getElapsedSinceLastChange() const -> core::Duration
{
  return m_elapsedSinceLastChange;
}

auto StatusComponent::tryGetElapsedSinceLastAppearing() const -> std::optional<core::Duration>
{
  return m_elapsedSinceAppearing;
}

auto StatusComponent::tryGetCurrentJumpTime() const -> core::Duration
{
  if (!m_currentJumpTime)
  {
    error("Failed to get current jump time", "No such value");
  }
  return *m_currentJumpTime;
}

auto StatusComponent::tryGetElapsedSinceJumpStarted() const -> core::Duration
{
  if (!m_elapsedSinceJumpStarted)
  {
    error("Failed to get elapsed since jump", "No such value");
  }
  return *m_elapsedSinceJumpStarted;
}

auto StatusComponent::tryGetRemainingJumpTime() const -> core::Duration
{
  const auto jumpTime = tryGetCurrentJumpTime();
  const auto elapsed  = tryGetElapsedSinceJumpStarted();

  return jumpTime - elapsed;
}

void StatusComponent::setStatus(const Status &status)
{
  updateJumpState(status, false);
  updateAppearingState(m_status);

  m_status                 = status;
  m_justChanged            = true;
  m_elapsedSinceLastChange = core::Duration{0};
}

void StatusComponent::update(const TickData &data)
{
  // TODO: We should not convert to milliseconds here.
  constexpr auto MILLISECONDS_IN_A_SECONDS = 1000;
  const auto elapsedMillis                 = core::Milliseconds(
    static_cast<int>(data.elapsed.toSeconds() * MILLISECONDS_IN_A_SECONDS));

  m_elapsedSinceLastChange += elapsedMillis;
  if (m_elapsedSinceAppearing)
  {
    *m_elapsedSinceAppearing += elapsedMillis;
  }
  if (m_elapsedSinceJumpStarted)
  {
    *m_elapsedSinceJumpStarted += elapsedMillis;
  }
}

void StatusComponent::updateJumpState(const Status &newStatus, const bool forceUpdate)
{
  if (!m_jumpTime || !m_threatJumpTime)
  {
    return;
  }

  const auto wasJumping = statusIndicatesJump(m_status) && !forceUpdate;
  const auto isJumping  = statusIndicatesJump(newStatus);
  if (!wasJumping && isJumping)
  {
    m_elapsedSinceJumpStarted = core::Duration{0};
    m_currentJumpTime         = statusIndicatesThreat(m_status) ? *m_threatJumpTime : *m_jumpTime;
  }
  if (wasJumping && !isJumping)
  {
    m_elapsedSinceJumpStarted.reset();
    m_currentJumpTime.reset();
  }
}

void StatusComponent::updateAppearingState(const Status &newStatus)
{
  if (m_elapsedSinceAppearing)
  {
    return;
  }

  if (statusIndicatesAppearing(newStatus))
  {
    m_elapsedSinceAppearing = core::Duration{0};
  }
}

} // namespace bsgo
