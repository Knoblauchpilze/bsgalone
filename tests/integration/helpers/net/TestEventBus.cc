
#include "TestEventBus.hh"

namespace test {

void TestEventBus::pushEvent(net::IEventPtr event)
{
  m_events.push_back(std::move(event));
  m_received.set_value(true);
}

void TestEventBus::addListener(net::IEventListenerPtr /*listener*/)
{
  throw std::runtime_error("Unexpected call to addListener on DummyEventBus");
}

bool TestEventBus::empty()
{
  throw std::runtime_error("Unexpected call to empty on DummyEventBus");
}

void TestEventBus::processEvents()
{
  throw std::runtime_error("Unexpected call to processEvents on DummyEventBus");
}

auto TestEventBus::waitForEvent() -> net::IEventPtr
{
  m_received         = std::promise<bool>();
  auto eventReceived = m_received.get_future();

  eventReceived.get();

  auto event = std::move(m_events.at(0));
  m_events.pop_front();
  return event;
}

} // namespace test
