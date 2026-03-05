
#include "INode.hh"

namespace bsgo {

INode::INode(const std::string &name)
  : core::CoreObject(name)
{
  setService("node");
}

} // namespace bsgo
