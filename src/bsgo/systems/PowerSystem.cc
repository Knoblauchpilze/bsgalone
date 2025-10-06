
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
                               const TickData &data) const
{
  if (canRegeneratePower(entity))
  {
    // TODO: We should use the tick duration as is.
    entity.powerComp().update(data.elapsed.toSeconds());
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
