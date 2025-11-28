
#include "AiComponent.hh"

namespace bsgo {

AiComponent::AiComponent(INodePtr behavior, DataContext context)
  : AbstractComponent(ComponentType::AI)
  , m_behavior(std::move(behavior))
  , m_context(std::move(context))
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

auto AiComponent::dataContext() const -> const DataContext &
{
  return m_context;
}

auto AiComponent::dataContext() -> DataContext &
{
  return m_context;
}

void AiComponent::update(const chrono::TickData & /*data*/) {}

} // namespace bsgo
