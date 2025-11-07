
#include "ConditionNode.hh"

namespace bsgo {

ConditionNode::ConditionNode(const Condition &condition)
  : INode("condition")
  , m_condition(condition)
{}

auto ConditionNode::tick(const BehaviorData &data) -> NodeState
{
  if (m_condition(data))
  {
    return NodeState::SUCCESS;
  }

  return NodeState::FAILED;
}

void ConditionNode::reset(DataContext & /*data*/) {}

} // namespace bsgo
