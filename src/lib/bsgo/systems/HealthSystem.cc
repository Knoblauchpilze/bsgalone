
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
                                Coordinator & /*coordinator*/,
                                const float elapsedSeconds) const
{
  entity.healthComp().update(elapsedSeconds);
}

} // namespace bsgo
