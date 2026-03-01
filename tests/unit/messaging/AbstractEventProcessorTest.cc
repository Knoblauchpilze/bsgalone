
#include "AbstractEventProcessor.hh"
#include "ConnectionMessage.hh"
#include "DockMessage.hh"
#include <chrono>
#include <future>
#include <gtest/gtest.h>

using namespace ::testing;

namespace messaging {
namespace {
enum class TestEventType
{
  EVENT_1,
  EVENT_2,
};

auto str(const TestEventType type) -> std::string
{
  return std::to_string(static_cast<int>(type));
}

class TestEvent
{
  public:
  TestEvent(const TestEventType type)
    : m_type(type)
  {}

  auto type() const -> TestEventType
  {
    return m_type;
  }

  private:
  TestEventType m_type{};
};

using TestEventPtr = std::unique_ptr<TestEvent>;

using TestProcessor = AbstractEventProcessor<TestEventType, TestEvent>;

auto createProcessor(std::deque<TestEventPtr> &events,
                     std::mutex &locker,
                     TestProcessor::EventHandler handler) -> TestProcessor
{
  return TestProcessor("test", events, locker, std::move(handler), {});
}
} // namespace

TEST(Unit_Messaging_AbstractEventProcessor, DoesNotCallHandlerWhenNoEventsAreAvailable)
{
  std::deque<TestEventPtr> noEvents{};
  std::mutex locker{};

  std::atomic_int control{0};
  auto handler = [&control](const TestEvent & /*event*/) { control.fetch_add(1); };

  auto processor = createProcessor(noEvents, locker, handler);

  processor.processEvents();

  EXPECT_EQ(0, control.load());
}

TEST(Unit_Messaging_AbstractEventProcessor, CallsHandlerForEachAvailableEvent)
{
  std::deque<TestEventPtr> events;
  events.push_back(std::make_unique<TestEvent>(TestEventType::EVENT_1));
  events.push_back(std::make_unique<TestEvent>(TestEventType::EVENT_2));
  std::mutex locker{};

  std::atomic_int control{0};
  auto handler = [&control](const TestEvent & /*event*/) { control.fetch_add(1); };

  auto processor = createProcessor(events, locker, handler);

  processor.processEvents();

  EXPECT_EQ(2, control.load());
}

TEST(Unit_Messaging_AbstractEventProcessor, ForwardsAvailableEventToHandler)
{
  std::deque<TestEventPtr> events;
  events.push_back(std::make_unique<TestEvent>(TestEventType::EVENT_1));

  std::mutex locker{};

  auto handler = [](const TestEvent &event) { EXPECT_EQ(TestEventType::EVENT_1, event.type()); };

  auto processor = createProcessor(events, locker, handler);

  processor.processEvents();
}

TEST(Unit_Messaging_AbstractEventProcessor, AcquiresProvidedLockBeforeProcessing)
{
  std::deque<TestEventPtr> events;
  events.push_back(std::make_unique<TestEvent>(TestEventType::EVENT_1));

  std::mutex locker{};

  std::atomic_int control{0};
  auto handler = [&control](const TestEvent & /*event*/) { control.fetch_add(1); };

  // Lock the mutex to prevent processing
  locker.lock();

  auto processor = createProcessor(events, locker, handler);

  // Trigger the processing of the events in a separate thread so that we
  // can verify that the handler is not called while the thread is locked
  auto started       = std::promise<bool>();
  auto threadStarted = started.get_future();

  auto result = std::async(std::launch::async, [&processor, &started]() {
    started.set_value(true);
    processor.processEvents();
  });
  threadStarted.get();

  EXPECT_EQ(0, control.load());

  // Release the mutex: this should allow events processing to happen
  locker.unlock();

  // We should have received a call to `processEvents`
  result.get();
  EXPECT_EQ(1, control.load());
}

} // namespace messaging
