
#include "PickTargetNode.hh"
#include "CircleBox.hh"
#include "Coordinator.hh"

namespace bsgo {

PickTargetNode::PickTargetNode()
  : LeafNode("pick")
{}

void PickTargetNode::run(const TickData &data)
{
  constexpr auto ENEMY_DETECTION_RANGE = 5.3f;

  const auto box     = CircleBox(data.ent.transformComp().position(), ENEMY_DETECTION_RANGE);
  const auto enemies = data.coordinator
                         .getEntitiesWithinSatistying(box, [&data](const Entity &entity) {
                           if (EntityKind::SHIP != entity.kind->kind())
                           {
                             return false;
                           }
                           if (data.ent.factionComp().faction() == entity.factionComp().faction())
                           {
                             return false;
                           }
                           return statusVisibleFromDradis(entity.statusComp().status());
                         });

  if (enemies.empty())
  {
    fail();
    return;
  }

  auto best      = std::numeric_limits<float>::max();
  const auto pos = data.ent.transformComp().position();
  for (const auto &enemy : enemies)
  {
    const auto enemyPos = enemy.transformComp().position();
    const auto d        = (enemyPos - pos).norm();

    if (d < best)
    {
      data.ent.targetComp().setTarget(enemy.uuid);
      best = d;
    }
  }

  log("Picked enemy target " + str(*data.ent.targetComp().target()));
  finish();
}

} // namespace bsgo
