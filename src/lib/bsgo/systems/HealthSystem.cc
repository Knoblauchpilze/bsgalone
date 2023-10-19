
#include "HealthSystem.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &ent)
{
  return ent.exists<HealthComponent>();
}
} // namespace

HealthSystem::HealthSystem()
  : AbstractSystem("health", isEntityRelevant)
{}

void HealthSystem::updateEntity(Entity &entity,
                                const Coordinator & /*coordinator*/,
                                const float elapsedSeconds)
{
  entity.access<HealthComponent>().update(elapsedSeconds);
}

} // namespace bsgo
