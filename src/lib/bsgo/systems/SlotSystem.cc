
#include "SlotSystem.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &entity)
{
  return !entity.weapons.empty();
}
} // namespace

SlotSystem::SlotSystem()
  : AbstractSystem("slot", isEntityRelevant)
{}

void SlotSystem::updateEntity(Entity &entity,
                              const Coordinator & /*coordinator*/,
                              const float elapsedSeconds)
{
  for (const auto &weapon : entity.weapons)
  {
    weapon->update(elapsedSeconds);
  }
}

} // namespace bsgo
