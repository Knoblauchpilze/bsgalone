
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
                               const Coordinator & /*coordinator*/,
                               const float elapsedSeconds)
{
  entity.access<PowerComponent>().update(elapsedSeconds);
}

} // namespace bsgo
