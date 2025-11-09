
#include "LeafNode.hh"

namespace bsgo {

LeafNode::LeafNode(const std::string &name)
  : INode(name)
{
  addModule("leaf");
}

auto LeafNode::tick(const BehaviorData &data) -> NodeState
{
  run(data);
  return m_state;
}

void LeafNode::reset(DataContext &data)
{
  m_state = NodeState::IDLE;
  resetInternal(data);
}

void LeafNode::start()
{
  m_state = NodeState::RUNNING;
}

void LeafNode::fail()
{
  m_state = NodeState::FAILED;
}

void LeafNode::finish()
{
  m_state = NodeState::SUCCESS;
}

void LeafNode::resetInternal(DataContext & /*data*/)
{
  // Intentionally empty.
}

} // namespace bsgo
