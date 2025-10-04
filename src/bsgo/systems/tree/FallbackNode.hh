
#pragma once

#include "ControlNode.hh"

namespace bsgo {

class FallbackNode : public ControlNode
{
  public:
  FallbackNode();
  ~FallbackNode() override = default;

  auto tick(const BehaviorData &data) -> NodeState override;
};

} // namespace bsgo
