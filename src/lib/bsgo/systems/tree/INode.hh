
#pragma once

#include "NodeState.hh"
#include "TickData.hh"
#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class INode : public utils::CoreObject
{
  public:
  INode(const std::string &name);
  ~INode() override = default;

  virtual auto tick(const TickData &data) -> NodeState = 0;
  virtual void reset()                                 = 0;
};

using INodePtr = std::unique_ptr<INode>;

} // namespace bsgo
