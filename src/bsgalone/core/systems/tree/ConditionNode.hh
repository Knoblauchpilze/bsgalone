
#pragma once

#include "INode.hh"
#include <functional>

namespace bsgo {

using Condition = std::function<bool(const BehaviorData &)>;

class ConditionNode : public INode
{
  public:
  ConditionNode(const Condition &condition);
  ~ConditionNode() override = default;

  auto tick(const BehaviorData &data) -> NodeState override;
  void reset(DataContext &data) override;

  private:
  Condition m_condition;
};

} // namespace bsgo
