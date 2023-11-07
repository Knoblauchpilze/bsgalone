
#include "SequenceNode.hh"

namespace bsgo {

SequenceNode::SequenceNode()
  : ControlNode("sequence")
{}

auto SequenceNode::tick(const TickData &data) -> NodeState
{
  for (const auto &child : m_children)
  {
    const auto status = child->tick(data);
    switch (status)
    {
      case NodeState::RUNNING:
        return NodeState::RUNNING;
      case NodeState::FAILED:
        return NodeState::FAILED;
      default:
        break;
    }
  }

  return NodeState::SUCCESS;
}

} // namespace bsgo
