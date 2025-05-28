
#pragma once

#include "CoreObject.hh"
#include "NodeState.hh"
#include "TickData.hh"
#include <memory>

namespace bsgo {

/// https://www.gamedeveloper.com/programming/behavior-trees-for-ai-how-they-work
/// https://en.wikipedia.org/wiki/Behavior_tree_(artificial_intelligence,_robotics_and_control)
class INode : public core::CoreObject
{
  public:
  INode(const std::string &name);
  ~INode() override = default;

  /// @brief - Process the node and returns the computed state.
  /// @param data - data to help the processing.
  /// @return - the state of the node.
  virtual auto tick(const TickData &data) -> NodeState = 0;

  /// @brief - Reset the node to its initial state.
  virtual void reset() = 0;
};

using INodePtr = std::unique_ptr<INode>;

} // namespace bsgo
