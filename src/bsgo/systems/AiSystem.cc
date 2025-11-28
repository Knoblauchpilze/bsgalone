
#include "AiSystem.hh"
#include "AiBehaviorSyncMessage.hh"
#include "Coordinator.hh"
#include "TargetMessage.hh"

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
    triggerAiBehaviorSync(entity, coordinator);
  }
}

void AiSystem::triggerAiBehaviorSync(Entity &entity, Coordinator &coordinator) const
{
  trySendAiBehaviorSyncMessage(entity);
  trySendTargetMessage(entity, coordinator);

  auto &aiComp = entity.aiComp();
  aiComp.dataContext().markAsSynced();
}

namespace {
bool needsToSendBehaviorSyncMessage(const DataContext &context)
{
  const auto maybeTargetIndex = context.tryGetKey(ContextKey::TARGET_REACHED);
  return maybeTargetIndex && maybeTargetIndex->changed();
}
} // namespace

void AiSystem::trySendAiBehaviorSyncMessage(const Entity &entity) const
{
  auto &aiComp = entity.aiComp();
  if (!needsToSendBehaviorSyncMessage(aiComp.dataContext()))
  {
    return;
  }

  const auto entityDbId = entity.dbComp().dbId();
  auto out              = std::make_unique<AiBehaviorSyncMessage>(entityDbId);

  const auto maybeTargetIndex = aiComp.dataContext().tryGetKey(ContextKey::TARGET_REACHED);
  if (maybeTargetIndex && maybeTargetIndex->changed())
  {
    out->setTargetIndex(maybeTargetIndex->as<Uuid>());
  }

  pushInternalMessage(std::move(out));
}

namespace {
bool needsToSendTargetMessage(const DataContext &context)
{
  const auto maybeTarget = context.tryGetKey(ContextKey::PICKED_TARGET);
  return maybeTarget && maybeTarget->changed();
}
} // namespace

void AiSystem::trySendTargetMessage(const Entity &entity, Coordinator &coordinator) const
{
  auto &aiComp = entity.aiComp();
  if (!needsToSendTargetMessage(aiComp.dataContext()))
  {
    return;
  }

  const auto dummyPosition = Eigen::Vector3f::Zero();
  TargetData data{
    .sourceDbId = entity.dbComp().dbId(),
    .sourceKind = entity.kind->kind(),
  };

  const auto maybeTarget = entity.targetComp().target();
  if (maybeTarget)
  {
    const auto target = coordinator.getEntity(*maybeTarget);
    data.targetDbId   = target.dbComp().dbId();
    data.targetKind   = target.kind->kind();
  }

  auto out = std::make_unique<TargetMessage>(data, dummyPosition);
  pushInternalMessage(std::move(out));
}

} // namespace bsgo
