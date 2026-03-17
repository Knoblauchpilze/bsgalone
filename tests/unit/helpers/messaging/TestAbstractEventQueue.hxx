
#include "TestAbstractEventQueue.hh"

namespace test {

template<messaging::EventTypeLike EventType, messaging::EventLike<EventType> Event>
void TestAbstractEventQueue<EventType, Event>::pushEvent(Base::EventPtr message)
{
  const std::lock_guard guard(m_locker);
  m_messages.push_back(std::move(message));
}

template<messaging::EventTypeLike EventType, messaging::EventLike<EventType> Event>
void TestAbstractEventQueue<EventType, Event>::addListener(Base::EventListenerPtr /*listener*/)
{
  throw std::runtime_error("Unsupported operation addListener in TestMessageQueue");
}

template<messaging::EventTypeLike EventType, messaging::EventLike<EventType> Event>
bool TestAbstractEventQueue<EventType, Event>::empty()
{
  const std::lock_guard guard(m_locker);
  return m_messages.empty();
}

template<messaging::EventTypeLike EventType, messaging::EventLike<EventType> Event>
void TestAbstractEventQueue<EventType, Event>::processEvents()
{
  throw std::runtime_error("Unsupported operation processMessages in TestMessageQueue");
}

template<messaging::EventTypeLike EventType, messaging::EventLike<EventType> Event>
auto TestAbstractEventQueue<EventType, Event>::messages() const
  -> const std::vector<typename Base::EventPtr> &
{
  return m_messages;
}

template<messaging::EventTypeLike EventType, messaging::EventLike<EventType> Event>
void TestAbstractEventQueue<EventType, Event>::clearMessages()
{
  const std::lock_guard guard(m_locker);
  return m_messages.clear();
}

} // namespace test
