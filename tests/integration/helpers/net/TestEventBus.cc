
#include "TestEventBus.hh"

namespace test {

void TestEventBus::pushEvent(net::IEventPtr event)
{
  std::unique_lock guard(m_locker);
  m_events.push_back(std::move(event));
  m_notifier.notify_one();
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

auto TestEventBus::waitForEvents() -> std::vector<net::IEventPtr>
{
  std::unique_lock guard(m_locker);

  if (!m_events.empty())
  {
    std::vector<net::IEventPtr> events{};
    std::swap(events, m_events);
    return events;
  }

  // This timeout should be enough for most test scenario. If it becomes
  // an issue it can be made configurable.
  constexpr auto REASONABLE_TIMEOUT = std::chrono::seconds(5);
  m_notifier.wait_for(guard, REASONABLE_TIMEOUT, [this] { return !m_events.empty(); });

  std::vector<net::IEventPtr> events{};
  std::swap(events, m_events);
  return events;
}

namespace {
auto nameAllEvents(const std::vector<net::IEventPtr> &events) -> std::string
{
  std::string out;
  if (events.empty())
  {
    return "";
  }

  for (std::size_t id = 0u; id < events.size(); ++id)
  {
    {
      out += net::str(events[id]->type());
      if (id < events.size() - 1u)
      {
        out += " ";
      }
    }
  }
  return out;
}
} // namespace

auto TestEventBus::waitForEvent() -> net::IEventPtr
{
  auto events = waitForEvents();
  if (events.size() != 1u)
  {
    throw std::runtime_error("Expected a single event to be returned but got "
                             + std::to_string(events.size()) + ": " + nameAllEvents(events));
  }

  net::IEventPtr out{};
  std::swap(out, events.at(0));

  return out;
}

auto TestEventBus::waitForEvent(const net::EventType type, const int maxTries) -> net::IEventPtr
{
  int tryCount = 0;
  net::IEventPtr out;

  while (out == nullptr && tryCount < maxTries)
  {
    auto events           = waitForEvents();
    const auto maybeEvent = std::find_if(events.begin(),
                                         events.end(),
                                         [type](const net::IEventPtr &event) {
                                           return event->type() == type;
                                         });
    if (maybeEvent != events.end())
    {
      std::swap(out, *maybeEvent);
    }

    ++tryCount;
  }

  if (!out)
  {
    throw std::runtime_error("No " + net::str(type) + " received, tried " + std::to_string(tryCount)
                             + " time(s)");
  }
  return out;
}

} // namespace test
