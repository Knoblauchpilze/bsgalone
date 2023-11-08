
#include "NodeUtils.hh"

namespace bsgo {

bool moveTowardsTarget(Entity &entity, const Eigen::Vector3f &target)
{
  const auto &transform                        = entity.transformComp();
  const Eigen::Vector3f toTarget               = target - transform.position();
  const auto d                                 = toTarget.norm();
  constexpr auto THRESHOLD_TO_ARRIVE_AT_TARGET = 0.5f;
  if (d < THRESHOLD_TO_ARRIVE_AT_TARGET)
  {
    return true;
  }

  auto &velocity = entity.velocityComp();
  velocity.accelerate(toTarget.normalized());

  return false;
}

} // namespace bsgo
