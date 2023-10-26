
#include "LootSystem.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &entity)
{
  return entity.exists<LootComponent>() && entity.exists<HealthComponent>();
}

} // namespace

LootSystem::LootSystem()
  : AbstractSystem("loot", isEntityRelevant)
{}

void LootSystem::updateEntity(Entity &entity,
                              Coordinator &coordinator,
                              const float /*elapsedSeconds*/) const
{
  const auto &health = entity.healthComp();
  if (health.isAlive())
  {
    return;
  }

  const auto &loot      = entity.lootComp();
  const auto recipients = loot.recipients();

  for (const auto &recipient : recipients)
  {
    const auto ent = coordinator.getEntity(recipient);
    warn("shoud distribute to " + ent.str());
  }
}

} // namespace bsgo
