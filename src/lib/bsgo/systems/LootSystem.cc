
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
    distributeLootTo(recipient, coordinator);
  }
}

void LootSystem::distributeLootTo(const Uuid &recipient, Coordinator &coordinator) const
{
  const auto ent = coordinator.getEntity(recipient);
  if (!ent.exists<PlayerComponent>())
  {
    return;
  }

  const auto player = coordinator.getEntity(ent.playerComp().player());
  if (player.exists<LockerComponent>())
  {
    warn("Entity " + ent.str() + " has player component but no locker");
    return;
  }

  warn("shoud distribute to " + str(player.uuid));
}

} // namespace bsgo
