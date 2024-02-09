
#include "FollowTargetNode.hh"
#include "Coordinator.hh"
#include "NodeUtils.hh"

#include "VectorUtils.hh"

namespace bsgo {

FollowTargetNode::FollowTargetNode()
  : LeafNode("follow")
{}

void FollowTargetNode::run(const TickData &data)
{
  if (NodeState::IDLE == m_state)
  {
    verbose("Trying to reach target of " + data.ent.str());
    start();
  }
  if (NodeState::RUNNING != m_state)
  {
    return;
  }

  const auto &targetComp = data.ent.targetComp();
  if (!targetComp.target())
  {
    verbose(data.ent.str() + " doesn't have target anymore");
    fail();
    return;
  }

  const auto target = data.coordinator.getEntity(*targetComp.target());
  if (!target.valid())
  {
    debug(data.ent.str() + " is not alive anymore");
    fail();
    return;
  }

  const auto targetPosition = target.transformComp().position();

  const auto reachedTarget = moveTowardsTarget(data.ent, targetPosition);
  if (reachedTarget)
  {
    verbose("Reached target " + target.str());
    finish();
  }
}

} // namespace bsgo
