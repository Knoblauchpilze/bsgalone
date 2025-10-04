
#pragma once

#include "INode.hh"

namespace bsgo {

class LeafNode : public INode
{
  public:
  LeafNode(const std::string &name);
  ~LeafNode() override = default;

  auto tick(const BehaviorData &data) -> NodeState override;
  void reset() override;

  protected:
  NodeState m_state{NodeState::IDLE};

  void start();
  virtual void run(const BehaviorData &data) = 0;
  void fail();
  void finish();
};

} // namespace bsgo
