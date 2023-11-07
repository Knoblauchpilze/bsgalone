
#include "AISystem.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &ent)
{
  return ent.exists<AIComponent>();
}
} // namespace

AISystem::AISystem()
  : AbstractSystem("ai", isEntityRelevant)
{}

void AISystem::updateEntity(Entity &entity,
                            Coordinator & /*coordinator*/,
                            const float elapsedSeconds) const
{
  auto &aiComp = entity.aiComp();
  aiComp.update(elapsedSeconds);

  TickData data{.ent = entity};
  aiComp.behavior().tick(data);
}

} // namespace bsgo
