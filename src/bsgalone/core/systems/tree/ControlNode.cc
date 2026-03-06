
#include "ControlNode.hh"

namespace bsgalone::core {

ControlNode::ControlNode(const std::string &name)
  : INode(name)
{}

void ControlNode::addChild(INodePtr node)
{
  m_children.push_back(std::move(node));
}

void ControlNode::reset(DataContext &data)
{
  for (const auto &child : m_children)
  {
    child->reset(data);
  }
}

} // namespace bsgalone::core
