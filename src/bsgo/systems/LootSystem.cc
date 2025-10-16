
#include "LootSystem.hh"
#include "Coordinator.hh"
#include "LootMessage.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &entity)
{
  return entity.exists<LootComponent>() && entity.exists<HealthComponent>();
}
} // namespace

LootSystem::LootSystem()
  : AbstractSystem(SystemType::LOOT, isEntityRelevant)
{}

void LootSystem::updateEntity(Entity &entity,
                              Coordinator &coordinator,
                              const chrono::TickData &data) const
{
  entity.lootComp().update(data);

  const auto &health = entity.healthComp();
  if (health.isAlive())
  {
    return;
  }

  if (entity.kind->kind() != EntityKind::ASTEROID)
  {
    error("Failed to distribute loot", "Expected asteroid but got " + entity.str());
  }

  distributeLoot(entity, coordinator);
}

void LootSystem::distributeLoot(const Entity &entity, Coordinator &coordinator) const
{
  const auto &loot      = entity.lootComp();
  const auto recipients = loot.recipients();

  for (const auto &recipient : recipients)
  {
    distributeLootTo(recipient, entity, coordinator);
  }
}

void LootSystem::distributeLootTo(const Uuid recipient,
                                  const Entity &deadTarget,
                                  Coordinator &coordinator) const
{
  const auto ent = coordinator.getEntity(recipient);
  if (!ent.valid())
  {
    debug("Skipping " + ent.str() + " as it is dead");
    return;
  }
  if (!ent.exists<OwnerComponent>())
  {
    debug("Skipping " + ent.str() + " as it is not a player");
    return;
  }

  auto player = coordinator.getEntity(ent.ownerComp().owner());

  distributeResourcesTo(player, deadTarget);
}

void LootSystem::distributeResourcesTo(const Entity &player, const Entity &deadTarget) const
{
  const auto playerDbId = player.dbComp().dbId();

  for (const auto &resource : deadTarget.resources)
  {
    info("Distributing " + std::to_string(resource->amount()) + " of " + str(resource->resource())
         + " to " + player.str());

    pushInternalMessage(
      std::make_unique<LootMessage>(playerDbId, resource->resource(), resource->amount()));
  }
}

} // namespace bsgo
