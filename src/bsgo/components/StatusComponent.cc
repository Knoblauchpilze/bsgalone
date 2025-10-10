
#include "StatusComponent.hh"

namespace bsgo {

StatusComponent::StatusComponent(const Status &status,
                                 const std::optional<TickDuration> &jumpTime,
                                 const std::optional<TickDuration> &threatJumpTime)
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

auto StatusComponent::jumpTime() const -> TickDuration
{
  if (!m_jumpTime)
  {
    error("Failed to get jump time", "No such value");
  }
  return *m_jumpTime;
}

auto StatusComponent::threatJumpTime() const -> TickDuration
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
  m_elapsedSinceAppearing = TickDuration();
}

auto StatusComponent::getElapsedSinceLastChange() const -> TickDuration
{
  return m_elapsedSinceLastChange;
}

auto StatusComponent::tryGetElapsedSinceLastAppearing() const -> std::optional<TickDuration>
{
  return m_elapsedSinceAppearing;
}

auto StatusComponent::getCurrentJumpTime() const -> TickDuration
{
  if (!m_currentJumpTime)
  {
    error("Failed to get current jump time", "No such value");
  }
  return *m_currentJumpTime;
}

auto StatusComponent::getElapsedSinceJumpStarted() const -> TickDuration
{
  if (!m_elapsedSinceJumpStarted)
  {
    error("Failed to get elapsed since jump", "No such value");
  }
  return *m_elapsedSinceJumpStarted;
}

auto StatusComponent::getRemainingJumpTime() const -> TickDuration
{
  const auto jumpTime = getCurrentJumpTime();
  const auto elapsed  = getElapsedSinceJumpStarted();

  return jumpTime - elapsed;
}

void StatusComponent::setStatus(const Status &status)
{
  updateJumpState(status, false);
  updateAppearingState(m_status);

  m_status                 = status;
  m_justChanged            = true;
  m_elapsedSinceLastChange = TickDuration();
}

void StatusComponent::update(const TickData &data)
{
  m_elapsedSinceLastChange += data.elapsed;
  if (m_elapsedSinceAppearing)
  {
    *m_elapsedSinceAppearing += data.elapsed;
  }
  if (m_elapsedSinceJumpStarted)
  {
    *m_elapsedSinceJumpStarted += data.elapsed;
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
    m_elapsedSinceJumpStarted = TickDuration();
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
    m_elapsedSinceAppearing = TickDuration();
  }
}

} // namespace bsgo
