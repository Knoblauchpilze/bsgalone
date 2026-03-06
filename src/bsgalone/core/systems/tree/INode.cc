
#include "INode.hh"

namespace bsgalone::core {

INode::INode(const std::string &name)
  : ::core::CoreObject(name)
{
  setService("node");
}

} // namespace bsgalone::core
