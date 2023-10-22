
#include "PowerSystem.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &ent)
{
  return ent.exists<PowerComponent>();
}
} // namespace

PowerSystem::PowerSystem()
  : AbstractSystem("power", isEntityRelevant)
{}

void PowerSystem::updateEntity(Entity &entity,
                               Coordinator & /*coordinator*/,
                               const float elapsedSeconds) const
{
  entity.powerComp().update(elapsedSeconds);
}

} // namespace bsgo
