
#include "PickTargetNode.hh"
#include "CircleBox.hh"
#include "Coordinator.hh"

namespace bsgo {

PickTargetNode::PickTargetNode()
  : LeafNode("pick")
{}

namespace {
constexpr auto ENEMY_DETECTION_RANGE = 15.0f;

bool isEntityTargetable(const Entity &entity, const Faction sourceFaction)
{
  if (bsgalone::core::EntityKind::SHIP != entity.kind->kind())
  {
    return false;
  }
  if (sourceFaction == entity.factionComp().faction())
  {
    return false;
  }

  return statusVisibleFromDradis(entity.statusComp().status());
}

auto pickClosestTarget(const std::vector<bsgo::Entity> &enemies, const Eigen::Vector3f &pos)
  -> Entity
{
  auto best = std::numeric_limits<float>::max();
  std::optional<Entity> maybeTarget{};

  for (const auto &enemy : enemies)
  {
    const auto enemyPos = enemy.transformComp().position();
    const auto d        = (enemyPos - pos).norm();

    if (d < best)
    {
      best        = d;
      maybeTarget = enemy;
    }
  }

  if (!maybeTarget)
  {
    throw std::invalid_argument("Failed to determine closest target among "
                                + std::to_string(enemies.size()));
  }

  return *maybeTarget;
}
} // namespace

void PickTargetNode::run(const BehaviorData &data)
{
  const auto box     = CircleBox(data.ent.transformComp().position(), ENEMY_DETECTION_RANGE);
  const auto faction = data.ent.factionComp().faction();

  const auto enemies = data.coordinator
                         .getEntitiesWithinSatistying(box, [faction](const Entity &entity) {
                           return isEntityTargetable(entity, faction);
                         });

  if (enemies.empty())
  {
    data.context.clear(ContextKey::PICKED_TARGET);
    fail();
    return;
  }

  const auto pos    = data.ent.transformComp().position();
  const auto target = pickClosestTarget(enemies, pos);

  updateTargetIfNeeded(data, target);

  finish();
}

void PickTargetNode::updateTargetIfNeeded(const BehaviorData &data, const Entity &target) const
{
  auto &targetComp = data.ent.targetComp();

  const std::optional<Uuid> newTarget{target.uuid};
  if (newTarget == targetComp.target())
  {
    return;
  }

  verbose("Picked enemy target " + target.str());
  data.ent.targetComp().setTarget(target.uuid);

  data.context.setKey(ContextKey::PICKED_TARGET, target.uuid);
}

} // namespace bsgo
