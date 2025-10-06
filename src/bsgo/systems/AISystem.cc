
#include "AISystem.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &ent)
{
  return ent.exists<AIComponent>();
}
} // namespace

AISystem::AISystem()
  : AbstractSystem(SystemType::AI, isEntityRelevant)
{}

void AISystem::updateEntity(Entity &entity, Coordinator &coordinator, const TickData &data) const
{
  auto &aiComp = entity.aiComp();
  aiComp.update(data);

  BehaviorData aiData{.ent = entity, .coordinator = coordinator};
  aiComp.behavior().tick(aiData);
}

} // namespace bsgo
