
#include "SynchronizedEventBus.hh"
#include "ClientConnectedEvent.hh"
#include <future>
#include <gtest/gtest.h>

using namespace ::testing;

namespace net {
namespace {
class DummyEventListener : public IEventListener
{
  public:
  DummyEventListener(const EventType relevantEvent)
    : m_relevantEvent(relevantEvent)
  {}

  ~DummyEventListener() override = default;

  bool isEventRelevant(const EventType &type) const
  {
    return type == m_relevantEvent;
  }

  void onEventReceived(const IEvent &event)
  {
    m_called.fetch_add(1);

    const std::lock_guard guard(m_lock);
    m_capturedEvents.push_back(event.clone());
  }

  auto calledCount() const -> int
  {
    return m_called.load();
  }

  auto events() const -> const std::vector<IEventPtr> &
  {
    return m_capturedEvents;
  }

  private:
  EventType m_relevantEvent{};
  std::atomic_int m_called{};

  std::mutex m_lock{};
  std::vector<IEventPtr> m_capturedEvents{};
};
} // namespace

TEST(Unit_Net_Messaging_SynchronizedEventBus, EmptyWhenNoEventAvailable)
{
  SynchronizedEventBus bus;
  EXPECT_TRUE(bus.empty());
}

TEST(Unit_Net_Messaging_SynchronizedEventBus, NotEmptyWhenAnEventIsAvailable)
{
  SynchronizedEventBus bus;
  bus.pushEvent(std::make_unique<ClientConnectedEvent>(ClientId{3}));

  EXPECT_FALSE(bus.empty());
}

TEST(Unit_Net_Messaging_SynchronizedEventBus, DoesNotCallListenerWhenNoEventIsAvailable)
{
  SynchronizedEventBus bus;

  auto listener    = std::make_unique<DummyEventListener>(EventType::DATA_RECEIVED);
  auto rawListener = listener.get();
  bus.addListener(std::move(listener));

  bus.processEvents();

  EXPECT_EQ(0, rawListener->calledCount());
}

TEST(Unit_Net_Messaging_SynchronizedEventBus, DoesNotCallListenerWhenEventIsNotRelevant)
{
  SynchronizedEventBus bus;

  auto listener    = std::make_unique<DummyEventListener>(EventType::DATA_RECEIVED);
  auto rawListener = listener.get();
  bus.addListener(std::move(listener));

  bus.pushEvent(std::make_unique<ClientConnectedEvent>(ClientId{3}));

  bus.processEvents();

  EXPECT_EQ(0, rawListener->calledCount());
}

TEST(Unit_Net_Messaging_SynchronizedEventBus, CallsListenerWhenEventIsRelevant)
{
  SynchronizedEventBus bus;

  auto listener    = std::make_unique<DummyEventListener>(EventType::CLIENT_CONNECTED);
  auto rawListener = listener.get();
  bus.addListener(std::move(listener));

  bus.pushEvent(std::make_unique<ClientConnectedEvent>(ClientId{3}));

  bus.processEvents();

  EXPECT_EQ(1, rawListener->calledCount());
}

TEST(Unit_Net_Messaging_SynchronizedEventBus, ForwardsEventToListener)
{
  SynchronizedEventBus bus;

  auto listener    = std::make_unique<DummyEventListener>(EventType::CLIENT_CONNECTED);
  auto rawListener = listener.get();
  bus.addListener(std::move(listener));

  bus.pushEvent(std::make_unique<ClientConnectedEvent>(ClientId{3}));

  bus.processEvents();

  const auto &actualEvents = rawListener->events();

  EXPECT_EQ(1u, actualEvents.size());
  const auto &event = actualEvents.front();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, event->type());
  EXPECT_EQ(ClientId{3}, event->as<ClientConnectedEvent>().clientId());
}

TEST(Unit_Net_Messaging_SynchronizedEventBus, NotifiesMultipleListenersForTheSameEvent)
{
  SynchronizedEventBus bus;

  auto listener1    = std::make_unique<DummyEventListener>(EventType::CLIENT_CONNECTED);
  auto rawListener1 = listener1.get();
  bus.addListener(std::move(listener1));

  auto listener2    = std::make_unique<DummyEventListener>(EventType::CLIENT_CONNECTED);
  auto rawListener2 = listener2.get();
  bus.addListener(std::move(listener2));

  bus.pushEvent(std::make_unique<ClientConnectedEvent>(ClientId{3}));

  bus.processEvents();

  EXPECT_EQ(1, rawListener1->calledCount());
  EXPECT_EQ(1, rawListener2->calledCount());
}

namespace {
class SlowEventListener : public IEventListener
{
  public:
  SlowEventListener(const EventType relevantEvent, std::future<bool> checkPoint)
    : m_relevantEvent(relevantEvent)
    , m_checkPoint(std::move(checkPoint))
  {}

  ~SlowEventListener() override = default;

  bool isEventRelevant(const EventType &type) const
  {
    return type == m_relevantEvent;
  }

  void onEventReceived(const IEvent &event)
  {
    m_called.fetch_add(1);
    m_checkPoint.get();

    const std::lock_guard guard(m_lock);
    m_capturedEvents.push_back(event.clone());
  }

  auto calledCount() const -> int
  {
    return m_called.load();
  }

  auto events() const -> const std::vector<IEventPtr> &
  {
    return m_capturedEvents;
  }

  private:
  EventType m_relevantEvent{};
  std::atomic_int m_called{};
  std::future<bool> m_checkPoint;

  std::mutex m_lock{};
  std::vector<IEventPtr> m_capturedEvents{};
};
} // namespace

TEST(Unit_Net_Messaging_SynchronizedEventBus, WaitsEndOfProcessingToAddNewEvent)
{
  SynchronizedEventBus bus;

  std::promise<bool> blocker;

  auto listener    = std::make_unique<SlowEventListener>(EventType::CLIENT_CONNECTED,
                                                      blocker.get_future());
  auto rawListener = listener.get();
  bus.addListener(std::move(listener));

  bus.pushEvent(std::make_unique<ClientConnectedEvent>(ClientId{3}));

  // Start processing of events: this will block until the slow listener
  // is released from waiting on the promise
  std::promise<bool> processingLaunch;
  auto processingLaunched = processingLaunch.get_future();

  auto processingFinished = std::async(std::launch::async, [&bus, &processingLaunch]() {
    processingLaunch.set_value(true);
    bus.processEvents();
  });

  // Start the publishing of an event: it should conflict with the slow
  // listener and wait
  std::promise<bool> enqueuingLaunch;
  auto enqueuingLaunched = enqueuingLaunch.get_future();

  auto enqueuingFinished = std::async(std::launch::async, [&bus, &enqueuingLaunch]() {
    enqueuingLaunch.set_value(true);
    bus.pushEvent(std::make_unique<ClientConnectedEvent>(ClientId{4}));
  });

  processingLaunched.get();
  enqueuingLaunched.get();

  // Release the processing
  blocker.set_value(true);

  processingFinished.get();
  enqueuingFinished.get();

  // The listener should have received only the first event
  const auto &actualEvents = rawListener->events();

  EXPECT_EQ(1u, actualEvents.size());
  const auto &event = actualEvents.front();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, event->type());
  EXPECT_EQ(ClientId{3}, event->as<ClientConnectedEvent>().clientId());
}

} // namespace net
