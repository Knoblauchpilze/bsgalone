
#pragma once

#include "INode.hh"

namespace bsgo {

class RepeaterNode : public INode
{
  public:
  RepeaterNode(INodePtr node);
  ~RepeaterNode() override = default;

  auto tick(const BehaviorData &data) -> NodeState override;
  void reset(DataContext &data) override;

  private:
  INodePtr m_child{};
};

} // namespace bsgo
