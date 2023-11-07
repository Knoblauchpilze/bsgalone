
#include "RepeaterNode.hh"

namespace bsgo {

RepeaterNode::RepeaterNode(INodePtr node)
  : INode("repeater")
  , m_child(std::move(node))
{}

auto RepeaterNode::tick(const TickData &data) -> NodeState
{
  const auto state = m_child->tick(data);
  if (NodeState::SUCCESS == state)
  {
    log("Child succeeded, reset and repeat");
    m_child->reset();
    return NodeState::RUNNING;
  }

  return state;
}

void RepeaterNode::reset()
{
  m_child->reset();
}

} // namespace bsgo
