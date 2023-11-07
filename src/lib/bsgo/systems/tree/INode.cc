
#include "INode.hh"

namespace bsgo {

INode::INode(const std::string &name)
  : utils::CoreObject(name)
{
  setService("node");
}

} // namespace bsgo
