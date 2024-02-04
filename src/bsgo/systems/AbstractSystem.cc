
#include "AbstractSystem.hh"
#include "Coordinator.hh"

namespace bsgo {

AbstractSystem::AbstractSystem(const SystemType &type, const EntityPredicate &entitiesFilter)
  : ISystem(str(type))
  , m_systemType(type)
  , m_entitiesFilter(entitiesFilter)
{}

auto AbstractSystem::type() const -> SystemType
{
  return m_systemType;
}

void AbstractSystem::installInternalMessageQueue(IMessageQueue *messageQueue)
{
  m_internalMessageQueue = messageQueue;
}

void AbstractSystem::installOutputMessageQueue(IMessageQueue *messageQueue)
{
  m_outputMessageQueue = messageQueue;
}

void AbstractSystem::update(Coordinator &coordinator, const float elapsedSeconds) const
{
  /// https://gamedev.stackexchange.com/questions/71711/ecs-how-to-access-multiple-components-not-the-same-one-in-a-system
  auto entities = coordinator.getEntitiesSatistying(m_entitiesFilter);
  for (auto &ent : entities)
  {
    updateEntity(ent, coordinator, elapsedSeconds);
  }
}

void AbstractSystem::pushInternalMessage(IMessagePtr message) const
{
  if (nullptr == m_internalMessageQueue)
  {
    error("Failed to push message", "Internal message queue is not set");
  }

  m_internalMessageQueue->pushMessage(std::move(message));
}

void AbstractSystem::pushMessage(IMessagePtr message) const
{
  if (nullptr == m_outputMessageQueue)
  {
    error("Failed to push message", "Output message queue is not set");
  }

  m_outputMessageQueue->pushMessage(std::move(message));
}

} // namespace bsgo
