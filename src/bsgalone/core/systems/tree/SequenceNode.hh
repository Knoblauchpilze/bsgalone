
#pragma once

#include "ControlNode.hh"

namespace bsgo {

class SequenceNode : public ControlNode
{
  public:
  SequenceNode();
  ~SequenceNode() override = default;

  auto tick(const BehaviorData &data) -> NodeState override;
};

} // namespace bsgo
