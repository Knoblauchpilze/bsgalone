
#include "AiSystem.hh"
#include "AiBehaviorSyncMessage.hh"

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
    triggerAiBehaviorSync(entity);
  }
}

void AiSystem::triggerAiBehaviorSync(Entity &entity) const
{
  auto &aiComp = entity.aiComp();

  aiComp.dataContext().markAsSynced();

  const auto entityDbId = entity.dbComp().dbId();
  auto out              = std::make_unique<AiBehaviorSyncMessage>(entityDbId);

  const auto maybeTargetIndex = aiComp.dataContext().tryGetKey<Uuid>(ContextKey::TARGET_REACHED);
  if (maybeTargetIndex)
  {
    out->setTargetIndex(*maybeTargetIndex);
  }

  pushInternalMessage(std::move(out));
}

} // namespace bsgo
