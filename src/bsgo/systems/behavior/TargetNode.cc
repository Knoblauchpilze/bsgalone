
#include "TargetNode.hh"
#include "Entity.hh"
#include "NodeUtils.hh"
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
    verbose("Trying to reach " + str(m_target));
    start();
  }
  if (NodeState::RUNNING != m_state)
  {
    return;
  }

  const auto reachedTarget = moveTowardsTarget(data.ent, m_target);
  if (reachedTarget)
  {
    verbose("Reached target " + str(m_target));
    finish();
  }
}

} // namespace bsgo
