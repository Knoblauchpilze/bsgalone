
#include "LootSystem.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &entity)
{
  return entity.exists<LootComponent>();
}

} // namespace

LootSystem::LootSystem()
  : AbstractSystem("loot", isEntityRelevant)
{}

void LootSystem::updateEntity(Entity & /*entity*/,
                              Coordinator & /*coordinator*/,
                              const float /*elapsedSeconds*/) const
{}

} // namespace bsgo
