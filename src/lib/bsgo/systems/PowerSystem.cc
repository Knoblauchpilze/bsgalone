
#include "PowerSystem.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &ent)
{
  return ent.exists<PowerComponent>();
}
} // namespace

PowerSystem::PowerSystem()
  : AbstractSystem(SystemType::POWER, isEntityRelevant)
{}

void PowerSystem::updateEntity(Entity &entity,
                               Coordinator & /*coordinator*/,
                               const float elapsedSeconds) const
{
  if (canRegeneratePower(entity))
  {
    entity.powerComp().update(elapsedSeconds);
  }
}

bool PowerSystem::canRegeneratePower(Entity &entity) const
{
  if (!entity.exists<StatusComponent>())
  {
    return true;
  }
  return statusAllowsPowerRegeneration(entity.statusComp().status());
}

} // namespace bsgo
