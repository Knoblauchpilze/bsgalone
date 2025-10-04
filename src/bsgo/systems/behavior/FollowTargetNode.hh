
#pragma once

#include "LeafNode.hh"

namespace bsgo {

class FollowTargetNode : public LeafNode
{
  public:
  FollowTargetNode();
  ~FollowTargetNode() override = default;

  protected:
  void run(const BehaviorData &data) override;
};

} // namespace bsgo
