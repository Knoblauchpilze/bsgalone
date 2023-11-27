
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

void StatusComponent::setStatus(const Status &status)
{
  m_status                 = status;
  m_justChanged            = true;
  m_elapsedSinceLastChange = utils::Duration{0};
}

void StatusComponent::update(const float elapsedSeconds)
{
  constexpr auto MILLISECONDS_IN_A_SECONDS = 1000;
  m_elapsedSinceLastChange += utils::Milliseconds(
    static_cast<int>(elapsedSeconds * MILLISECONDS_IN_A_SECONDS));
}

} // namespace bsgo
