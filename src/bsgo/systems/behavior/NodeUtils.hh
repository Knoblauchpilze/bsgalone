
#pragma once

#include "Entity.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgo {

bool moveTowardsTarget(Entity &entity, const Eigen::Vector3f &target);

}
