
#pragma once

#include "INode.hh"
#include <functional>

namespace bsgo {

using Condition = std::function<bool(TickData)>;

class ConditionNode : public INode
{
  public:
  ConditionNode(const Condition &condition);
  ~ConditionNode() override = default;

  auto tick(const TickData &data) -> NodeState override;
  void reset() override;

  private:
  Condition m_condition;
};

} // namespace bsgo
