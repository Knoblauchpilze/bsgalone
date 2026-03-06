
#pragma once

#include "LeafNode.hh"

namespace bsgalone::core {

class PickTargetNode : public LeafNode
{
  public:
  PickTargetNode();
  ~PickTargetNode() override = default;

  protected:
  void run(const BehaviorData &data) override;

  private:
  void updateTargetIfNeeded(const BehaviorData &data, const Entity &maybeTarget) const;
};

} // namespace bsgalone::core
