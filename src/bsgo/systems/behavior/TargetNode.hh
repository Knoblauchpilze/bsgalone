
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

  private:
  int m_index{};
  Eigen::Vector3f m_target{};
};

} // namespace bsgo
