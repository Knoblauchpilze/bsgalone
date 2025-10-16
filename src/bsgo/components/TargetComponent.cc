
#include "TargetComponent.hh"

namespace bsgo {

TargetComponent::TargetComponent()
  : AbstractComponent(ComponentType::TARGET)
{}

TargetComponent::TargetComponent(const Uuid target)
  : AbstractComponent(ComponentType::TARGET)
  , m_target(target)
{}

auto TargetComponent::target() const -> std::optional<Uuid>
{
  return m_target;
}

void TargetComponent::clearTarget()
{
  m_target.reset();
}

void TargetComponent::setTarget(const Uuid target)
{
  m_target = target;
}

void TargetComponent::update(const chrono::TickData & /*data*/) {}

} // namespace bsgo
