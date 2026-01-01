
#include "TestEventBus.hh"

#include <iostream>

namespace test {

void TestEventBus::pushEvent(net::IEventPtr event)
{
  std::cout << "[bus] got event " << str(event->type()) << "\n";
  m_events.push_back(std::move(event));
  std::cout << "[bus] satisfying promise\n";
  m_received.set_value(true);
  std::cout << "[bus] registered event\n";
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

  std::cout << "[bus] waiting for event\n";

  // This timeout should be enough for most test scenario. If it becomes
  // an issue it can be made configurable.
  constexpr auto REASONABLE_TIMEOUT = std::chrono::seconds(5);
  eventReceived.wait_for(REASONABLE_TIMEOUT);

  std::cout << "[bus] fulfilled promise\n";

  auto event = std::move(m_events.at(0));
  m_events.pop_front();
  return event;
}

} // namespace test
