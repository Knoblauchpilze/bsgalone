
#include "AIComponent.hh"

namespace bsgo {

AIComponent::AIComponent(INodePtr behavior)
  : IComponent("ai")
  , m_behavior(std::move(behavior))
{
  if (nullptr == m_behavior)
  {
    throw std::invalid_argument("Expected non null behavior tree");
  }
}

auto AIComponent::behavior() -> INode &
{
  return *m_behavior;
}

auto AIComponent::behavior() const -> const INode &
{
  return *m_behavior;
}

void AIComponent::update(const float /*elapsedSeconds*/) {}

} // namespace bsgo
