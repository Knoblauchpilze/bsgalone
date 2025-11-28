
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

void TargetNode::resetInternal(DataContext &data)
{
  // Clearing the data context guarantees that the next execution of the
  // `determineCompletionState` method will not use the current target as
  // a sign that this target was already reached.
  // Note: this means that we also override the progress of any other node
  // relying on this target. We could have some scoping/namespace mechanism
  // if behavior trees become more complex.
  data.clear(ContextKey::TARGET_REACHED);
}

void TargetNode::determineCompletionState(DataContext &context)
{
  if (NodeState::IDLE != m_state)
  {
    return;
  }

  const auto maybeTargetIndex = context.tryGetKey<Uuid>(ContextKey::TARGET_REACHED);
  if (maybeTargetIndex && *maybeTargetIndex > m_index)
  {
    verbose("Target was already reached, setting node to succesful");
    finish();
    return;
  }

  verbose("Trying to reach " + str(m_target) + ", with index " + std::to_string(m_index));
  start();

  // When we first execute the node we need to indicate that a new target
  // is now active in the context. This allows external listeners to be
  // aware that the target changed. It can be used for example to send the
  // information to client applications.
  context.setKey(ContextKey::TARGET_REACHED, m_index);
}

} // namespace bsgo
