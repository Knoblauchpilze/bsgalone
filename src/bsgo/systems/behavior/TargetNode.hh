
#pragma once

#include "LeafNode.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgo {

class TargetNode : public LeafNode
{
  public:
  /// @brief - Creates a new target node which will try to reach a certain position.
  /// @param target - the position to reach
  /// @param index - used for reporting to update the data context when this node is
  /// first activated.
  TargetNode(const Eigen::Vector3f &target, const int index);
  ~TargetNode() override = default;

  protected:
  void run(const BehaviorData &data) override;

  /// @brief - Override allowing to clear the reached target from the data context.
  /// This will make sure that the next execution of the node will get a chance to
  /// check whether it should execute itself.
  /// If a behavior tree contains several such nodes, the clearing of the reached
  /// target will be triggered multiple times. This is ok.
  /// @param data - the context containing the target
  void resetInternal(DataContext &data) override;

  private:
  int m_index{};
  Eigen::Vector3f m_target{};

  void determineCompletionState(DataContext &context);
};

} // namespace bsgo
