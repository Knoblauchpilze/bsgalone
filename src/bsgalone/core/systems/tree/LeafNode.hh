
#pragma once

#include "INode.hh"

namespace bsgo {

class LeafNode : public INode
{
  public:
  LeafNode(const std::string &name);
  ~LeafNode() override = default;

  auto tick(const BehaviorData &data) -> NodeState override;
  void reset(DataContext &data) override;

  protected:
  NodeState m_state{NodeState::IDLE};

  void start();
  virtual void run(const BehaviorData &data) = 0;
  void fail();
  void finish();

  /// @brief - Allows inheriting classes to specialize the reset behavior and
  /// perform additional changes on their internal state or on the behavior
  /// data. This method is guaranteed to be call whenever `reset` is called,
  /// after the `LeafNode` base class has performed its reset.
  /// The default implementation is empty but can be overloaded as needed.
  /// @param data - the data as passed to the `reset` method
  virtual void resetInternal(DataContext &data);
};

} // namespace bsgo
