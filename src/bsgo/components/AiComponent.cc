
#include "AiComponent.hh"

namespace bsgo {

AiComponent::AiComponent(INodePtr behavior)
  : AbstractComponent(ComponentType::AI)
  , m_behavior(std::move(behavior))
{
  if (nullptr == m_behavior)
  {
    throw std::invalid_argument("Expected non null behavior tree");
  }
}

auto AiComponent::behavior() -> INode &
{
  return *m_behavior;
}

auto AiComponent::behavior() const -> const INode &
{
  return *m_behavior;
}

void AiComponent::update(const chrono::TickData & /*data*/) {}

} // namespace bsgo
