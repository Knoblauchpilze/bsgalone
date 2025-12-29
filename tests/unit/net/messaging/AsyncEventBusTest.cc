
#include "AsyncEventBus.hh"
#include "ClientConnectedEvent.hh"
#include <future>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

namespace net {
class MockEventBus : public IEventBus
{
  public:
  MockEventBus()
  {
    // Those calls are set by default as the async processing starts immediately
    // and there's a chance that the event bus is called.
    ON_CALL(*this, empty()).WillByDefault(Return(true));
  }

  ~MockEventBus() override = default;

  MOCK_METHOD(void, pushEvent, (IEventPtr), (override));
  MOCK_METHOD(void, addListener, (IEventListenerPtr), (override));
  MOCK_METHOD(bool, empty, (), (override));

  MOCK_METHOD(void, processEvents, (), (override));
};

TEST(Unit_Net_Messaging_AsyncEventBus, RequiresNonNullWrappedEventBus)
{
  EXPECT_THROW([]() { AsyncEventBus bus(nullptr); }(), std::invalid_argument);
}

TEST(Unit_Net_Messaging_AsyncEventBus, DelegatesEmpty)
{
  auto mock = std::make_unique<NiceMock<MockEventBus>>();
  EXPECT_CALL(*mock, empty()).Times(1).WillOnce(Return(true));

  AsyncEventBus bus(std::move(mock));

  EXPECT_TRUE(bus.empty());
}

namespace {
class DummyEventListener : public IEventListener
{
  public:
  DummyEventListener()           = default;
  ~DummyEventListener() override = default;

  bool isEventRelevant(const EventType & /*type*/) const
  {
    return false;
  }

  void onEventReceived(const IEvent & /*event*/) {}
};
} // namespace

TEST(Unit_Net_Messaging_AsyncEventBus, DelegatesAddListener)
{
  auto listener    = std::make_unique<DummyEventListener>();
  auto rawListener = listener.get();

  auto mock = std::make_unique<NiceMock<MockEventBus>>();
  EXPECT_CALL(*mock, addListener(_)).Times(1).WillOnce([&rawListener](IEventListenerPtr actual) {
    EXPECT_EQ(rawListener, actual.get());
  });

  AsyncEventBus bus(std::move(mock));

  bus.addListener(std::move(listener));
}

TEST(Unit_Net_Messaging_AsyncEventBus, DelegatesPushEvent)
{
  auto event    = std::make_unique<ClientConnectedEvent>(ClientId{3});
  auto rawEvent = event.get();

  auto mock = std::make_unique<NiceMock<MockEventBus>>();

  EXPECT_CALL(*mock, pushEvent(_)).Times(1).WillOnce([&rawEvent](IEventPtr actual) {
    EXPECT_EQ(rawEvent, actual.get());
  });

  AsyncEventBus bus(std::move(mock));

  bus.pushEvent(std::move(event));
}

TEST(Unit_Net_Messaging_AsyncEventBus, ThrowsErrorWhenProcessingEventsIsCalled)
{
  EXPECT_THROW(
    []() {
      auto mock = std::make_unique<NiceMock<MockEventBus>>();
      AsyncEventBus bus(std::move(mock));

      bus.processEvents();
    }(),
    core::CoreException);
}

namespace {
class DummyEventBus : public IEventBus
{
  public:
  DummyEventBus()
    : m_processEventsCalled(m_processEvents.get_future())
  {}

  ~DummyEventBus() override = default;

  void pushEvent(IEventPtr event)
  {
    m_events.push_back(std::move(event));
  }

  void addListener(IEventListenerPtr /*listener*/) override
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
  std::vector<IEventPtr> m_events{};
  std::promise<bool> m_processEvents{};
  std::future<bool> m_processEventsCalled;
};
} // namespace

TEST(Unit_Net_Messaging_AsyncEventBus, PushingAnEventTriggersProcessing)
{
  auto eventBus    = std::make_unique<DummyEventBus>();
  auto rawEventBus = eventBus.get();
  AsyncEventBus bus(std::move(eventBus));

  bus.pushEvent(std::make_unique<ClientConnectedEvent>(ClientId{3}));

  rawEventBus->wait();
}

} // namespace net
