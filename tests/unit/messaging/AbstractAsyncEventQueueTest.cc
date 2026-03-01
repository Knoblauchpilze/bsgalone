
#include "AbstractAsyncEventQueue.hh"
#include "TestEvent.hh"
#include <future>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace messaging {
namespace {
using TestQueue      = AbstractAsyncEventQueue<TestEventType, TestEvent>;
using TestQueueShPtr = std::shared_ptr<TestQueue>;

class MockEventQueue : public IEventQueue<TestEventType, TestEvent>
{
  public:
  MockEventQueue()
  {
    // Those calls are set by default as the async processing starts immediately
    // and there's a chance that the event queue is called.
    ON_CALL(*this, empty()).WillByDefault(Return(true));
  }

  ~MockEventQueue() override = default;

  MOCK_METHOD(void, pushEvent, (TestEventPtr), (override));
  MOCK_METHOD(void, addListener, (EventListenerPtr), (override));
  MOCK_METHOD(bool, empty, (), (override));

  MOCK_METHOD(void, processEvents, (), (override));
};
} // namespace

TEST(Unit_Messaging_AbstractAsyncEventQueue, RequiresNonNullWrappedEventBus)
{
  EXPECT_THROW([]() { TestQueue queue(nullptr); }(), std::invalid_argument);
}

TEST(Unit_Messaging_AbstractAsyncEventQueue, DelegatesEmpty)
{
  auto mock = std::make_unique<NiceMock<MockEventQueue>>();
  // There can be more than one call as the async processing loop of the
  // AbstractAsyncEventQueue will also call `empty`.
  EXPECT_CALL(*mock, empty()).Times(AtLeast(1)).WillOnce(Return(true));

  TestQueue queue(std::move(mock));

  EXPECT_TRUE(queue.empty());
}

namespace {
class DummyEventListener : public IEventListener<TestEventType, TestEvent>
{
  public:
  DummyEventListener()           = default;
  ~DummyEventListener() override = default;

  bool isEventRelevant(const TestEventType & /*type*/) const
  {
    return false;
  }

  void onEventReceived(const TestEvent & /*event*/) {}
};
} // namespace

TEST(Unit_Messaging_AbstractAsyncEventQueue, DelegatesAddListener)
{
  auto listener    = std::make_unique<DummyEventListener>();
  auto rawListener = listener.get();

  auto mock = std::make_unique<NiceMock<MockEventQueue>>();
  EXPECT_CALL(*mock, addListener(_))
    .Times(1)
    .WillOnce([&rawListener](MockEventQueue::EventListenerPtr actual) {
      EXPECT_EQ(rawListener, actual.get());
    });

  TestQueue queue(std::move(mock));

  queue.addListener(std::move(listener));
}

TEST(Unit_Messaging_AbstractAsyncEventQueue, DelegatesPushEvent)
{
  auto event    = std::make_unique<TestEvent>(TestEventType::EVENT_1);
  auto rawEvent = event.get();

  auto mock = std::make_unique<NiceMock<MockEventQueue>>();

  EXPECT_CALL(*mock, pushEvent(_)).Times(1).WillOnce([&rawEvent](TestEventPtr actual) {
    EXPECT_EQ(rawEvent, actual.get());
  });

  TestQueue queue(std::move(mock));

  queue.pushEvent(std::move(event));
}

TEST(Unit_Messaging_AbstractAsyncEventQueue, ThrowsErrorWhenProcessingEventsIsCalled)
{
  EXPECT_THROW(
    []() {
      auto mock = std::make_unique<NiceMock<MockEventQueue>>();
      TestQueue queue(std::move(mock));

      queue.processEvents();
    }(),
    core::CoreException);
}

namespace {
class DummyEventQueue : public IEventQueue<TestEventType, TestEvent>
{
  public:
  DummyEventQueue()
    : m_processEventsCalled(m_processEvents.get_future())
  {}

  ~DummyEventQueue() override = default;

  void pushEvent(TestEventPtr event)
  {
    m_events.push_back(std::move(event));
  }

  void addListener(EventListenerPtr /*listener*/) override
  {
    throw std::runtime_error("Unexpected call to addListener");
  }

  bool empty() override
  {
    return m_events.empty();
  }

  void processEvents() override
  {
    m_events.clear();
    m_processEvents.set_value(true);
  }

  void wait()
  {
    m_processEventsCalled.get();
  }

  private:
  std::vector<TestEventPtr> m_events{};
  std::promise<bool> m_processEvents{};
  std::future<bool> m_processEventsCalled;
};
} // namespace

TEST(Unit_Messaging_AbstractAsyncEventQueue, PushingAnEventTriggersProcessing)
{
  auto eventQueue    = std::make_unique<DummyEventQueue>();
  auto rawEventQueue = eventQueue.get();
  TestQueue queue(std::move(eventQueue));

  queue.pushEvent(std::make_unique<TestEvent>(TestEventType::EVENT_1));

  rawEventQueue->wait();
}

} // namespace messaging
