
#pragma once

#include "ControlNode.hh"

namespace bsgalone::core {

class SequenceNode : public ControlNode
{
  public:
  SequenceNode();
  ~SequenceNode() override = default;

  auto tick(const BehaviorData &data) -> NodeState override;
};

} // namespace bsgalone::core
