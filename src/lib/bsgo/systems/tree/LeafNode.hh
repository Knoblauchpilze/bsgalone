
#pragma once

#include "INode.hh"

namespace bsgo {

class LeafNode : public INode
{
  public:
  LeafNode(const std::string &name);
  ~LeafNode() override = default;

  auto tick(const TickData &data) -> NodeState override;
  void reset() override;

  protected:
  NodeState m_state{NodeState::IDLE};

  void start();
  virtual void run(const TickData &data) = 0;
  void fail();
  void finish();
};

} // namespace bsgo
