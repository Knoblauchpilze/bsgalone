
#include "ComputerSystem.hh"

namespace bsgo {
namespace {
bool isEntityRelevant(const Entity &entity)
{
  return !entity.computers.empty();
}
} // namespace

ComputerSystem::ComputerSystem()
  : AbstractSystem("computer", isEntityRelevant)
{}

void ComputerSystem::updateEntity(Entity &entity,
                                  const Coordinator & /*coordinator*/,
                                  const float elapsedSeconds)
{
  for (const auto &computer : entity.computers)
  {
    computer->update(elapsedSeconds);
  }
}

} // namespace bsgo
