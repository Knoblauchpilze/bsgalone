
#include "AbstractSystem.hh"
#include "Coordinator.hh"

namespace bsgo {

AbstractSystem::AbstractSystem(const SystemType &type, const EntityPredicate &entitiesFilter)
  : AbstractSystem(type, entitiesFilter, true)
{}

AbstractSystem::AbstractSystem(const SystemType &type,
                               const EntityPredicate &entitiesFilter,
                               const bool filterDeadAndRemovedEntities)
  : ISystem(str(type))
  , m_systemType(type)
  , m_entitiesFilter(entitiesFilter)
  , m_filterDeadAndRemovedEntities(filterDeadAndRemovedEntities)
{}

auto AbstractSystem::type() const -> SystemType
{
  return m_systemType;
}

void AbstractSystem::installInternalMessageQueue(IMessageQueue *messageQueue)
{
  if (nullptr == messageQueue)
  {
    error("Expected non null internal message queue");
  }

  m_internalMessageQueue = messageQueue;
}

void AbstractSystem::installOutputMessageQueue(IMessageQueue *messageQueue)
{
  if (nullptr == messageQueue)
  {
    error("Expected non null output message queue");
  }

  m_outputMessageQueue = messageQueue;
}

void AbstractSystem::update(Coordinator &coordinator, const chrono::TickData &data) const
{
  /// https://gamedev.stackexchange.com/questions/71711/ecs-how-to-access-multiple-components-not-the-same-one-in-a-system
  auto entities = coordinator.getEntitiesSatistying(m_entitiesFilter);
  for (auto &ent : entities)
  {
    if (m_filterDeadAndRemovedEntities)
    {
      const auto dead    = ent.exists<StatusComponent>() && ent.statusComp().isDead();
      const auto removed = ent.exists<RemovalComponent>() && ent.removalComp().toBeDeleted();
      if (dead || removed)
      {
        continue;
      }
    }

    updateEntity(ent, coordinator, data);
  }
}

void AbstractSystem::pushInternalMessage(bsgalone::core::IMessagePtr message) const
{
  if (m_internalMessageQueue != nullptr)
  {
    m_internalMessageQueue->pushMessage(std::move(message));
  }
}

void AbstractSystem::pushMessage(bsgalone::core::IMessagePtr message) const
{
  if (m_outputMessageQueue != nullptr)
  {
    m_outputMessageQueue->pushMessage(std::move(message));
  }
}

} // namespace bsgo
