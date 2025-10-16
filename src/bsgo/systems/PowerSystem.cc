
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
                               const chrono::TickData &data) const
{
  if (canRegeneratePower(entity))
  {
    entity.powerComp().update(data);
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
