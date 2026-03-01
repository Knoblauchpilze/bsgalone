
#include "SynchronizedEventBus.hh"
#include "EventProcessor.hh"

namespace net {

SynchronizedEventBus::SynchronizedEventBus()
  : AbstractEventBus()
  , core::CoreObject("bus")
{
  setService("event");
  addModule("synchronized");
}

void SynchronizedEventBus::pushEvent(IEventPtr event)
{
  const std::lock_guard guard(m_locker);
  m_events.emplace_back(std::move(event));
}

void SynchronizedEventBus::addListener(IEventListenerPtr listener)
{
  this->AbstractEventBus::addListener(std::move(listener));
}

bool SynchronizedEventBus::empty()
{
  const std::lock_guard guard(m_locker);
  return m_events.empty();
}

void SynchronizedEventBus::processEvents()
{
  auto processor = createEventProcessor(getName(), m_events, m_locker, [this](const IEvent &event) {
    processEvent(event);
  });

  processor.processEvents();
}

void SynchronizedEventBus::processEvent(const IEvent &event) const
{
  // https://stackoverflow.com/questions/72841621/finding-all-the-values-with-given-key-for-multimap
  auto [it, end] = m_listenersTable.equal_range(event.type());
  for (; it != end; ++it)
  {
    it->second->onEventReceived(event);
  }
}

} // namespace net
