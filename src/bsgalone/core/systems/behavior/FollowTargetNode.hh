
#pragma once

#include "LeafNode.hh"

namespace bsgalone::core {

class FollowTargetNode : public LeafNode
{
  public:
  FollowTargetNode();
  ~FollowTargetNode() override = default;

  protected:
  void run(const BehaviorData &data) override;
};

} // namespace bsgalone::core
