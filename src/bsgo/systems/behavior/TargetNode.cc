
#include "TargetNode.hh"
#include "Entity.hh"
#include "NodeUtils.hh"
#include "VectorUtils.hh"

namespace bsgo {

TargetNode::TargetNode(const Eigen::Vector3f &target, const int index)
  : LeafNode("target")
  , m_index(index)
  , m_target(target)
{}

void TargetNode::run(const BehaviorData &data)
{
  determineCompletionState(data.context);

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

void TargetNode::determineCompletionState(DataContext &context)
{
  if (NodeState::IDLE != m_state)
  {
    return;
  }

  verbose("Trying to reach " + str(m_target));
  start();

  // When we first execute the node we need to indicate that a new target
  // is now active in the context. This allows external listeners to be
  // aware that the target changed. It can be used for example to send the
  // information to client applications.
  context.setTargetIndex(m_index);
}

} // namespace bsgo
