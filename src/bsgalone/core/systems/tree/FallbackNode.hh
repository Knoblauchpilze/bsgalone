
#pragma once

#include "ControlNode.hh"

namespace bsgalone::core {

class FallbackNode : public ControlNode
{
  public:
  FallbackNode();
  ~FallbackNode() override = default;

  auto tick(const BehaviorData &data) -> NodeState override;
};

} // namespace bsgalone::core
