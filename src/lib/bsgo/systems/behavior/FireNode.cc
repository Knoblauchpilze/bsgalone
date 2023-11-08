
#include "FireNode.hh"

namespace bsgo {

FireNode::FireNode()
  : LeafNode("fire")
{}

void FireNode::run(const TickData & /*data*/)
{
  warn("should fire");
}

} // namespace bsgo
