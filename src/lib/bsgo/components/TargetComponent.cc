
#include "TargetComponent.hh"

namespace bsgo {

TargetComponent::TargetComponent()
  : IComponent("target")
{}

auto TargetComponent::target() const -> std::optional<Uuid>
{
  return m_target;
}

void TargetComponent::clearTarget()
{
  m_target.reset();
}

void TargetComponent::setTarget(const Uuid &target)
{
  m_target = target;
}

void TargetComponent::update(const float /*elapsedSeconds*/) {}

} // namespace bsgo
