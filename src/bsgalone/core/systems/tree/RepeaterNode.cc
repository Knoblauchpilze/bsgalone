
#include "RepeaterNode.hh"

namespace bsgo {

RepeaterNode::RepeaterNode(INodePtr node)
  : INode("repeater")
  , m_child(std::move(node))
{}

auto RepeaterNode::tick(const BehaviorData &data) -> NodeState
{
  const auto state = m_child->tick(data);
  if (NodeState::SUCCESS == state)
  {
    verbose("Child succeeded, reset and repeat");
    m_child->reset(data.context);
    return NodeState::RUNNING;
  }

  return state;
}

void RepeaterNode::reset(DataContext &data)
{
  m_child->reset(data);
}

} // namespace bsgo
