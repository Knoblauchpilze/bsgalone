
#include "AiSystem.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &ent)
{
  return ent.exists<AiComponent>();
}
} // namespace

AiSystem::AiSystem()
  : AbstractSystem(SystemType::AI, isEntityRelevant)
{}

void AiSystem::updateEntity(Entity &entity,
                            Coordinator &coordinator,
                            const chrono::TickData &data) const
{
  auto &aiComp = entity.aiComp();
  aiComp.update(data);

  BehaviorData aiData{.ent = entity, .coordinator = coordinator};
  aiComp.behavior().tick(aiData);
}

} // namespace bsgo
