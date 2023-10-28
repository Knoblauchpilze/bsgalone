
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

  if (entity.kind->kind() != EntityKind::ASTEROID)
  {
    error("Failed to distribute loot", "Expected asteroid but got " + entity.str());
  }

  const auto &loot = entity.lootComp();
  distributeLoot(loot, coordinator);
}

void LootSystem::distributeLoot(const LootComponent &loot, Coordinator &coordinator) const
{
  const auto recipients = loot.recipients();

  for (const auto &recipient : recipients)
  {
    distributeLootTo(recipient, loot, coordinator);
  }
}

void LootSystem::distributeLootTo(const Uuid &recipient,
                                  const LootComponent &loot,
                                  Coordinator &coordinator) const
{
  const auto ent = coordinator.getEntity(recipient);
  if (!ent.exists<PlayerComponent>())
  {
    log("Skipping " + ent.str() + " as it is not a player");
    return;
  }

  const auto player = ent.playerComp();

  switch (loot.type())
  {
    case Item::RESOURCE:
      distributeResourceTo(player.player(), loot.loot(), coordinator);
      break;
    default:
      error("Failed to distribute loot", "Unsupported loot type " + str(loot.type()));
      break;
  }
}

void LootSystem::distributeResourceTo(const Uuid &player,
                                      const Uuid &loot,
                                      Coordinator &coordinator) const
{
  const auto repositories = coordinator.repositories();

  const auto lootData = repositories.asteroidLootRepository->findOneById(loot);
  auto resource       = repositories.playerResourceRepository->findOneByIdAndResource(player,
                                                                                lootData.resource);

  info("Distributing " + std::to_string(lootData.amount) + " of " + str(lootData.resource) + " to "
       + str(player));
  resource.amount += lootData.amount;

  repositories.playerResourceRepository->save(resource);
}

} // namespace bsgo
