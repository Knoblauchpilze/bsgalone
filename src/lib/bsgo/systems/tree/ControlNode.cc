
#include "ControlNode.hh"

namespace bsgo {

ControlNode::ControlNode(const std::string &name)
  : INode(name)
{}

void ControlNode::addChild(INodePtr node)
{
  m_children.push_back(std::move(node));
}

void ControlNode::reset()
{
  for (const auto &child : m_children)
  {
    child->reset();
  }
}

} // namespace bsgo
