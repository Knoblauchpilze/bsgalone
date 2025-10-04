
#pragma once

#include "LeafNode.hh"

namespace bsgo {

class PickTargetNode : public LeafNode
{
  public:
  PickTargetNode();
  ~PickTargetNode() override = default;

  protected:
  void run(const BehaviorData &data) override;
};

} // namespace bsgo
