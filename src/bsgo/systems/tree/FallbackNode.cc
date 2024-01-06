
#include "FallbackNode.hh"

namespace bsgo {

FallbackNode::FallbackNode()
  : ControlNode("fallback")
{}

auto FallbackNode::tick(const TickData &data) -> NodeState
{
  for (const auto &child : m_children)
  {
    const auto status = child->tick(data);
    switch (status)
    {
      case NodeState::RUNNING:
        return NodeState::RUNNING;
      case NodeState::SUCCESS:
        return NodeState::SUCCESS;
      default:
        break;
    }
  }

  return NodeState::FAILED;
}

} // namespace bsgo
