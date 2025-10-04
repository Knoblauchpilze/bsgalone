
#pragma once

#include "LeafNode.hh"

namespace bsgo {

class FireNode : public LeafNode
{
  public:
  FireNode();
  ~FireNode() override = default;

  protected:
  void run(const BehaviorData &data) override;
};

} // namespace bsgo
