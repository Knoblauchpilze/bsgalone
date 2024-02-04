
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
  if (m_internalMessageQueue != nullptr)
  {
    m_internalMessageQueue->pushMessage(std::move(message));
  }
}

void AbstractSystem::pushMessage(IMessagePtr message) const
{
  if (m_outputMessageQueue != nullptr)
  {
    m_outputMessageQueue->pushMessage(std::move(message));
  }
}

} // namespace bsgo
