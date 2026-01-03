
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
class CounterListener : public IEventListener
{
  public:
  CounterListener()           = default;
  ~CounterListener() override = default;

  bool isEventRelevant(const EventType &type) const
  {
    return type == EventType::CLIENT_CONNECTED;
  }

  void onEventReceived(const IEvent & /*event*/)
  {
    m_receivedCount.fetch_add(1);
  }

  auto messagesCount() const -> int
  {
    return m_receivedCount.load();
  }

  private:
  std::atomic_int m_receivedCount{};
};

struct Synchronization
{
  std::mutex lock{};
  std::condition_variable notifier{};
  bool ready{false};
  std::atomic_bool productionFinished{false};

  void notifyStart()
  {
    std::lock_guard guard(lock);
    ready = true;
    notifier.notify_all();
  }
};

using SynchronizationShPtr = std::shared_ptr<Synchronization>;

constexpr auto MULTIPLIER = 10000;

auto generateClientId(const int threadId, const int messageId) -> int
{
  return MULTIPLIER * threadId + messageId;
}

class Producer
{
  public:
  Producer(const int id,
           const int messagesCount,
           SynchronizationShPtr synchronization,
           IEventBusShPtr bus)
    : m_id(id)
    , m_messagesCount(messagesCount)
    , m_sync(std::move(synchronization))
    , m_bus(std::move(bus))
  {}

  ~Producer() = default;

  void start()
  {
    m_thread = std::thread([this]() {
      waitForReadySignal();

      for (int messageId = 0; messageId < m_messagesCount; ++messageId)
      {
        const auto clientId = generateClientId(m_id, messageId);
        m_bus->pushEvent(std::make_unique<ClientConnectedEvent>(ClientId{clientId}));
      }
    });
  }

  void join()
  {
    m_thread.join();
  }

  private:
  int m_id{};
  int m_messagesCount{};
  SynchronizationShPtr m_sync{};
  IEventBusShPtr m_bus{};
  std::thread m_thread{};

  void waitForReadySignal()
  {
    std::unique_lock guard(m_sync->lock);
    m_sync->notifier.wait(guard, [this] { return m_sync->ready; });
  }
};

using ProducerPtr = std::unique_ptr<Producer>;

class Consumer
{
  public:
  Consumer(const int id, SynchronizationShPtr synchronization, IEventBusShPtr bus)
    : m_id(id)
    , m_sync(std::move(synchronization))
    , m_bus(std::move(bus))
  {}

  ~Consumer() = default;

  void start()
  {
    m_thread = std::thread([this]() {
      waitForReadySignal();

      while (!m_bus->empty() || !m_sync->productionFinished.load())
      {
        m_bus->processEvents();
      }
    });
  }

  void join()
  {
    m_thread.join();
  }

  private:
  int m_id{};
  SynchronizationShPtr m_sync{};
  IEventBusShPtr m_bus{};
  std::thread m_thread{};

  void waitForReadySignal()
  {
    std::unique_lock guard(m_sync->lock);
    m_sync->notifier.wait(guard, [this] { return m_sync->ready; });
  }
};

using ConsumerPtr = std::unique_ptr<Consumer>;

} // namespace

struct TestCaseConcurrentProduction
{
  int messages{};
  int producers{};
};

using ConcurrentProduction = TestWithParam<TestCaseConcurrentProduction>;

TEST_P(ConcurrentProduction, run)
{
  const auto &param = GetParam();
  // Due to the way we calculate the client identifier we need the number of messages to be
  // less than this value otherwise there will be collisions.
  EXPECT_LE(param.messages, MULTIPLIER);

  auto bus = std::make_shared<SynchronizedEventBus>();

  auto listener    = std::make_unique<CounterListener>();
  auto rawListener = listener.get();
  bus->addListener(std::move(listener));

  auto sync = std::make_shared<Synchronization>();

  std::vector<ProducerPtr> producers{};
  for (int id = 0; id < param.producers; ++id)
  {
    auto producer = std::make_unique<Producer>(id, param.messages, sync, bus);
    producer->start();

    producers.emplace_back(std::move(producer));
  }

  sync->notifyStart();

  std::for_each(producers.begin(), producers.end(), [](const ProducerPtr &producer) {
    producer->join();
  });

  bus->processEvents();

  const auto expectedMessagesCount = param.messages * param.producers;
  EXPECT_EQ(expectedMessagesCount, rawListener->messagesCount());
}

INSTANTIATE_TEST_SUITE_P(Unit_Net_Messaging_SynchronizedEventBus,
                         ConcurrentProduction,
                         Values(TestCaseConcurrentProduction{.messages = 1, .producers = 1},
                                TestCaseConcurrentProduction{.messages = 10, .producers = 1},
                                TestCaseConcurrentProduction{.messages = 100, .producers = 1},
                                TestCaseConcurrentProduction{.messages = 1000, .producers = 1},
                                TestCaseConcurrentProduction{.messages = 1, .producers = 5},
                                TestCaseConcurrentProduction{.messages = 10, .producers = 5},
                                TestCaseConcurrentProduction{.messages = 100, .producers = 5},
                                TestCaseConcurrentProduction{.messages = 1000, .producers = 5},
                                TestCaseConcurrentProduction{.messages = 1, .producers = 10},
                                TestCaseConcurrentProduction{.messages = 10, .producers = 10},
                                TestCaseConcurrentProduction{.messages = 100, .producers = 10},
                                TestCaseConcurrentProduction{.messages = 1000, .producers = 10},
                                TestCaseConcurrentProduction{.messages = 1, .producers = 50},
                                TestCaseConcurrentProduction{.messages = 10, .producers = 50},
                                TestCaseConcurrentProduction{.messages = 100, .producers = 50},
                                TestCaseConcurrentProduction{.messages = 1000, .producers = 50}),
                         [](const TestParamInfo<TestCaseConcurrentProduction> &info) -> std::string {
                           return std::to_string(info.param.messages) + "_"
                                  + std::to_string(info.param.producers);
                         });

struct TestCaseConcurrentProductionConsumption
{
  int messages{};
  int producers{};
  int consumers{};
};

using ConcurrentProductionConsumption = TestWithParam<TestCaseConcurrentProductionConsumption>;

TEST_P(ConcurrentProductionConsumption, DISABLED_run)
{
  const auto &param = GetParam();
  // Due to the way we calculate the client identifier we need the number of messages to be
  // less than this value otherwise there will be collisions.
  ASSERT_LE(param.messages, MULTIPLIER);

  auto bus = std::make_shared<SynchronizedEventBus>();

  auto listener    = std::make_unique<CounterListener>();
  auto rawListener = listener.get();
  bus->addListener(std::move(listener));

  auto sync = std::make_shared<Synchronization>();

  std::vector<ProducerPtr> producers{};
  for (int id = 0; id < param.producers; ++id)
  {
    auto producer = std::make_unique<Producer>(id, param.messages, sync, bus);
    producer->start();

    producers.emplace_back(std::move(producer));
  }

  std::vector<ConsumerPtr> consumers{};
  for (int id = 0; id < param.consumers; ++id)
  {
    auto consumer = std::make_unique<Consumer>(id, sync, bus);
    consumer->start();

    consumers.emplace_back(std::move(consumer));
  }

  sync->notifyStart();

  std::for_each(producers.begin(), producers.end(), [](const ProducerPtr &producer) {
    producer->join();
  });
  sync->productionFinished.store(true);
  std::for_each(consumers.begin(), consumers.end(), [](const ConsumerPtr &consumer) {
    consumer->join();
  });

  const auto expectedMessagesCount = param.messages * param.producers;
  EXPECT_EQ(expectedMessagesCount, rawListener->messagesCount());
}

INSTANTIATE_TEST_SUITE_P(
  Unit_Net_Messaging_SynchronizedEventBus,
  ConcurrentProductionConsumption,
  Values(
    TestCaseConcurrentProductionConsumption{.messages = 1, .producers = 1, .consumers = 1},
    TestCaseConcurrentProductionConsumption{.messages = 10, .producers = 1, .consumers = 1},
    TestCaseConcurrentProductionConsumption{.messages = 100, .producers = 1, .consumers = 1},
    TestCaseConcurrentProductionConsumption{.messages = 1000, .producers = 1, .consumers = 1},
    TestCaseConcurrentProductionConsumption{.messages = 100, .producers = 5, .consumers = 10},
    TestCaseConcurrentProductionConsumption{.messages = 1000, .producers = 5, .consumers = 5},
    TestCaseConcurrentProductionConsumption{.messages = 5000, .producers = 5, .consumers = 2},
    TestCaseConcurrentProductionConsumption{.messages = 100, .producers = 10, .consumers = 10},
    TestCaseConcurrentProductionConsumption{.messages = 100, .producers = 10, .consumers = 20},
    TestCaseConcurrentProductionConsumption{.messages = 1000, .producers = 10, .consumers = 5},
    TestCaseConcurrentProductionConsumption{.messages = 1000, .producers = 10, .consumers = 20},
    TestCaseConcurrentProductionConsumption{.messages = 5000, .producers = 10, .consumers = 5},
    TestCaseConcurrentProductionConsumption{.messages = 5000, .producers = 10, .consumers = 30}),
  [](const TestParamInfo<TestCaseConcurrentProductionConsumption> &info) -> std::string {
    return std::to_string(info.param.messages) + "_" + std::to_string(info.param.producers) + "_"
           + std::to_string(info.param.consumers);
  });

} // namespace net
