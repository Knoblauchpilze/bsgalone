
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

  BehaviorData aiData{
    .ent         = entity,
    .coordinator = coordinator,
    // See the Data Context section of this article:
    // https://www.gamedeveloper.com/programming/behavior-trees-for-ai-how-they-work
    .context = aiComp.dataContext(),
  };

  aiComp.behavior().tick(aiData);

  if (aiComp.dataContext().changed())
  {
    // TODO: Should send a message
    warn("should sync AI component for target "
         + std::to_string(*aiComp.dataContext().tryGetTargetIndex()));
    aiComp.dataContext().markAsSynced();
  }
}

} // namespace bsgo
