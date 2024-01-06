
#pragma once

#include "LeafNode.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgo {

class TargetNode : public LeafNode
{
  public:
  TargetNode(const Eigen::Vector3f &target);
  ~TargetNode() override = default;

  protected:
  void run(const TickData &data) override;

  private:
  Eigen::Vector3f m_target;
};

} // namespace bsgo
