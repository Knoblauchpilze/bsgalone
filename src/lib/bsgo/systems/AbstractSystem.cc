
#include "AbstractSystem.hh"

namespace bsgo {

AbstractSystem::AbstractSystem(const SystemType &type,
                               const Coordinator::EntityPredicate &entitiesFilter)
  : ISystem(str(type))
  , m_systemType(type)
  , m_entitiesFilter(entitiesFilter)
{}

auto AbstractSystem::type() const -> SystemType
{
  return m_systemType;
}

void AbstractSystem::installMessageQueue(IMessageQueue *messageQueue)
{
  m_messageQueue = messageQueue;
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

void AbstractSystem::pushMessage(IMessagePtr message) const
{
  if (nullptr == m_messageQueue)
  {
    error("Failed to push message", "Message queue is not set");
  }

  m_messageQueue->pushMessage(std::move(message));
}

} // namespace bsgo
