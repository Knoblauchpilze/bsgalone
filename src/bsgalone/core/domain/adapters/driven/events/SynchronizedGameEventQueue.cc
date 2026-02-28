
#include "SynchronizedGameEventQueue.hh"
#include "GameEventProcessor.hh"

namespace bsgalone::core {

SynchronizedGameEventQueue::SynchronizedGameEventQueue()
  : AbstractGameEventQueue()
  , ::core::CoreObject("queue")
{
  setService("event");
  addModule("synchronized");
}

void SynchronizedGameEventQueue::publishEvent(IGameEventPtr event)
{
  const std::lock_guard guard(m_locker);
  m_events.emplace_back(std::move(event));
}

void SynchronizedGameEventQueue::addListener(IGameEventListenerPtr listener)
{
  this->AbstractGameEventQueue::addListener(std::move(listener));
}

bool SynchronizedGameEventQueue::empty()
{
  const std::lock_guard guard(m_locker);
  return m_events.empty();
}

void SynchronizedGameEventQueue::processEvents()
{
  GameEventProcessor processor(getName(), m_events, m_locker, [this](const IGameEvent &event) {
    processEvent(event);
  });

  processor.processEvents();
}

void SynchronizedGameEventQueue::processEvent(const IGameEvent &event) const
{
  auto [it, end] = m_listenersTable.equal_range(event.type());
  for (; it != end; ++it)
  {
    it->second->onEventReceived(event);
  }
}

} // namespace bsgalone::core
