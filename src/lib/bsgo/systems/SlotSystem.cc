
#include "SlotSystem.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &entity)
{
  return !entity.weapons.empty() || !entity.computers.empty();
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
  for (const auto &computer : entity.computers)
  {
    computer->update(elapsedSeconds);
  }
}

} // namespace bsgo
