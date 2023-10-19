
#include "AbstractSystem.hh"

namespace bsgo {

AbstractSystem::AbstractSystem(const std::string &name,
                               const Coordinator::EntityPredicate &entitiesFilter)
  : ISystem(name)
  , m_entitiesFilter(entitiesFilter)
{}

void AbstractSystem::update(const Coordinator &coordinator, const float elapsedSeconds)
{
  /// https://gamedev.stackexchange.com/questions/71711/ecs-how-to-access-multiple-components-not-the-same-one-in-a-system
  auto entities = coordinator.getEntitiesSatistying(m_entitiesFilter);
  for (auto &ent : entities)
  {
    updateEntity(ent, coordinator, elapsedSeconds);
  }
}

} // namespace bsgo
