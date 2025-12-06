
#include "FireNode.hh"
#include "Coordinator.hh"
#include "SystemUtils.hh"

namespace bsgo {

FireNode::FireNode()
  : LeafNode("fire")
{}

void FireNode::run(const BehaviorData &data)
{
  setFiringState();

  const auto state = determineFiringState(data);
  switch (state)
  {
    case FiringState::TARGET_REACHABLE:
      fireAllWeapons(data.ent);
      break;
    case FiringState::TARGET_IS_DEAD:
      verbose("Target of " + data.ent.str() + " is dead");
      finish();
      break;
    case FiringState::NO_TARGET:
      fail();
      return;
    case FiringState::TARGET_DOES_NOT_EXIST:
      verbose("Target of " + data.ent.str() + " does not exist anymore");
      fail();
      return;
    case FiringState::TARGET_IS_TOO_FAR:
      fail();
      return;
    default:
      error("Unsupported firing state (" + std::to_string(static_cast<int>(state)) + ")");
      break;
  }
}

void FireNode::setFiringState()
{
  if (NodeState::IDLE != m_state)
  {
    return;
  }

  start();
}

namespace {
bool canFireOnTarget(const Entity &ent, const float distanceToTarget)
{
  return std::any_of(ent.weapons.begin(),
                     ent.weapons.end(),
                     [distanceToTarget](const WeaponSlotComponentShPtr &weapon) {
                       return weapon->range() >= distanceToTarget;
                     });
}
} // namespace

auto FireNode::determineFiringState(const BehaviorData &data) const -> FiringState
{
  const auto &target = data.ent.targetComp().target();
  if (!target)
  {
    return FiringState::NO_TARGET;
  }

  const auto targetEntity = data.coordinator.getEntity(*target);

  if (!targetEntity.valid())
  {
    return FiringState::TARGET_DOES_NOT_EXIST;
  }

  if (!targetEntity.healthComp().isAlive())
  {
    return FiringState::TARGET_IS_DEAD;
  }

  const auto distance = distanceToTarget(data.ent, targetEntity);

  if (!canFireOnTarget(data.ent, distance))
  {
    return FiringState::TARGET_IS_TOO_FAR;
  }

  return FiringState::TARGET_REACHABLE;
}

void FireNode::fireAllWeapons(Entity &ent)
{
  for (const auto &weapon : ent.weapons)
  {
    if (!weapon->active())
    {
      weapon->toggle();
    }
    weapon->registerFireRequest();
  }
}

} // namespace bsgo
