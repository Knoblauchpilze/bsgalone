
#include "TargetNode.hh"
#include "Entity.hh"

#include "VectorUtils.hh"

namespace bsgo {

TargetNode::TargetNode(const Eigen::Vector3f &target)
  : LeafNode("target")
  , m_target(target)
{}

void TargetNode::run(const TickData &data)
{
  if (NodeState::IDLE == m_state)
  {
    log("Trying to reach " + str(m_target));
    start();
  }
  if (NodeState::RUNNING != m_state)
  {
    return;
  }

  const auto &transform                        = data.ent.transformComp();
  const Eigen::Vector3f toTarget               = m_target - transform.position();
  const auto d                                 = toTarget.norm();
  constexpr auto THRESHOLD_TO_ARRIVE_AT_TARGET = 0.5f;
  if (d < THRESHOLD_TO_ARRIVE_AT_TARGET)
  {
    log("Reached target " + str(m_target) + " (d: " + std::to_string(d) + ")");
    finish();
    return;
  }

  auto &velocity = data.ent.velocityComp();
  velocity.accelerate(toTarget.normalized());
}

} // namespace bsgo
