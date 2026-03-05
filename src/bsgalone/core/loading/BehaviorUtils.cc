
#include "BehaviorUtils.hh"
#include "FallbackNode.hh"
#include "FireNode.hh"
#include "FollowTargetNode.hh"
#include "PickTargetNode.hh"
#include "RepeaterNode.hh"
#include "SequenceNode.hh"
#include "TargetNode.hh"

namespace bsgo {

auto generateIdleSequence(const std::vector<Eigen::Vector3f> &targets) -> INodePtr
{
  auto idleSequence = std::make_unique<SequenceNode>();

  for (std::size_t id = 0u; id < targets.size(); ++id)
  {
    auto targetNode = std::make_unique<TargetNode>(targets[id], static_cast<int>(id));
    idleSequence->addChild(std::move(targetNode));
  }

  return idleSequence;
}

auto generateAiBehaviorTree(const std::vector<Eigen::Vector3f> &targets) -> INodePtr
{
  auto fire         = std::make_unique<FireNode>();
  auto followTarget = std::make_unique<FollowTargetNode>();
  auto pickTarget   = std::make_unique<PickTargetNode>();
  auto idleSequence = generateIdleSequence(targets);

  auto fireAndFollowFallback = std::make_unique<FallbackNode>();
  fireAndFollowFallback->addChild(std::move(fire));
  fireAndFollowFallback->addChild(std::move(followTarget));

  auto attackSequence = std::make_unique<SequenceNode>();
  attackSequence->addChild(std::move(pickTarget));
  attackSequence->addChild(std::move(fireAndFollowFallback));

  auto fallbackNode = std::make_unique<FallbackNode>();
  fallbackNode->addChild(std::move(attackSequence));
  fallbackNode->addChild(std::move(idleSequence));

  auto repeater = std::make_unique<RepeaterNode>(std::move(fallbackNode));

  return repeater;
}

} // namespace bsgo
