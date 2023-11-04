
#include "StatusComponent.hh"

namespace bsgo {

StatusComponent::StatusComponent(const Status &status)
  : IComponent("status")
  , m_status(status)
{}

auto StatusComponent::status() const -> Status
{
  return m_status;
}

auto StatusComponent::getElapsedSinceLastChange() const -> utils::Duration
{
  return m_elapsedSinceLastChange;
}

void StatusComponent::setStatus(const Status &status)
{
  m_status                 = status;
  m_elapsedSinceLastChange = utils::Duration{0};
}

void StatusComponent::update(const float elapsedSeconds)
{
  constexpr auto MILLISECONDS_IN_A_SECONDS = 1000;
  m_elapsedSinceLastChange += utils::Milliseconds(
    static_cast<int>(elapsedSeconds * MILLISECONDS_IN_A_SECONDS));
}

} // namespace bsgo
