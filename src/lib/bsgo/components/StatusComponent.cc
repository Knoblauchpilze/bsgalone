
#include "StatusComponent.hh"

namespace bsgo {

StatusComponent::StatusComponent(const Status &status,
                                 const std::optional<utils::Duration> &jumpTime,
                                 const std::optional<utils::Duration> &threatJumpTime)
  : IComponent("status")
  , m_status(status)
  , m_jumpTime(jumpTime)
  , m_threatJumpTime(threatJumpTime)
{}

auto StatusComponent::status() const -> Status
{
  return m_status;
}

auto StatusComponent::jumpTime() const -> utils::Duration
{
  if (!m_jumpTime)
  {
    error("Failed to get jump time", "No such value");
  }
  return *m_jumpTime;
}

auto StatusComponent::threatJumpTime() const -> utils::Duration
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

auto StatusComponent::getElapsedSinceLastChange() const -> utils::Duration
{
  return m_elapsedSinceLastChange;
}

auto StatusComponent::tryGetCurrentJumpTime() const -> utils::Duration
{
  if (!m_currentJumpTime)
  {
    error("Failed to get current jump time", "No such value");
  }
  return *m_currentJumpTime;
}

auto StatusComponent::tryGetElapsedSinceJumpStarted() const -> utils::Duration
{
  if (!m_elapsedSinceJumpStarted)
  {
    error("Failed to get elapsed since jump", "No such value");
  }
  return *m_elapsedSinceJumpStarted;
}

void StatusComponent::setStatus(const Status &status)
{
  updateJumpState(status);

  m_status                 = status;
  m_justChanged            = true;
  m_elapsedSinceLastChange = utils::Duration{0};
}

void StatusComponent::update(const float elapsedSeconds)
{
  constexpr auto MILLISECONDS_IN_A_SECONDS = 1000;
  const auto elapsedMillis                 = utils::Milliseconds(
    static_cast<int>(elapsedSeconds * MILLISECONDS_IN_A_SECONDS));

  m_elapsedSinceLastChange += elapsedMillis;
  if (m_elapsedSinceJumpStarted)
  {
    *m_elapsedSinceJumpStarted += elapsedMillis;
  }
}

void StatusComponent::updateJumpState(const Status &newStatus)
{
  if (!m_jumpTime || !m_threatJumpTime)
  {
    return;
  }

  const auto wasJumping = statusIndicatesJump(m_status);
  const auto isJumping  = statusIndicatesJump(newStatus);
  if (!wasJumping && isJumping)
  {
    m_elapsedSinceJumpStarted = utils::Duration{0};
    m_currentJumpTime         = statusIndicatesThreat(m_status) ? *m_threatJumpTime : *m_jumpTime;
  }
  else if (!isJumping)
  {
    m_elapsedSinceJumpStarted.reset();
    m_currentJumpTime.reset();
  }
}

} // namespace bsgo
