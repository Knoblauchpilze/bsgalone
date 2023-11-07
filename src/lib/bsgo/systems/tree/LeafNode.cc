
#include "LeafNode.hh"

namespace bsgo {

LeafNode::LeafNode(const std::string &name)
  : INode(name)
{
  setService("leaf");
}

auto LeafNode::tick(const TickData &data) -> NodeState
{
  run(data);
  return m_state;
}

void LeafNode::reset()
{
  m_state = NodeState::IDLE;
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

} // namespace bsgo
