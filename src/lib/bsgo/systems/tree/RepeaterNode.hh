
#pragma once

#include "INode.hh"

namespace bsgo {

class RepeaterNode : public INode
{
  public:
  RepeaterNode(INodePtr node);
  ~RepeaterNode() override = default;

  auto tick(const TickData &data) -> NodeState override;
  void reset() override;

  private:
  INodePtr m_child{};
};

} // namespace bsgo
