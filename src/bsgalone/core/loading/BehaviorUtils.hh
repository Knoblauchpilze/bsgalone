
#pragma once

#include "INode.hh"
#include <eigen3/Eigen/Eigen>
#include <vector>

namespace bsgalone::core {

auto generateAiBehaviorTree(const std::vector<Eigen::Vector3f> &targets) -> INodePtr;

}
