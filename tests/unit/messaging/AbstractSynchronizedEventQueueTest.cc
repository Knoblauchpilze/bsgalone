
#include "AbstractSynchronizedEventQueue.hh"
#include "TestEvent.hh"
#include <condition_variable>
#include <gtest/gtest.h>
#include <thread>

using namespace test;
using namespace ::testing;

namespace messaging {
namespace {
using TestQueue      = AbstractSynchronizedEventQueue<TestEventType, TestEvent>;
using TestQueueShPtr = std::shared_ptr<TestQueue>;
} // namespace

TEST(Unit_Messaging_AbstractSynchronizedEventQueue, EmptyWhenNoEventAvailable)
{
  TestQueue queue(allEventTypesAsSet(), {});
  EXPECT_TRUE(queue.empty());
}

TEST(Unit_Messaging_AbstractSynchronizedEventQueue, NotEmptyWhenAnEventIsAvailable)
{
  TestQueue queue(allEventTypesAsSet(), {});
  queue.pushEvent(std::make_unique<TestEvent>(TestEventType::EVENT_1));

  EXPECT_FALSE(queue.empty());
}

namespace {
class DummyEventListener : public IEventListener<TestEventType, TestEvent>
{
  public:
  DummyEventListener(const TestEventType relevantEvent)
    : m_relevantEvent(relevantEvent)
  {}

  ~DummyEventListener() override = default;

  bool isEventRelevant(const TestEventType &type) const
  {
    return type == m_relevantEvent;
  }

  void onEventReceived(const TestEvent &event)
  {
    m_called.fetch_add(1);

    const std::lock_guard guard(m_lock);
    m_capturedEvents.push_back(event.clone());
  }

  auto calledCount() const -> int
  {
    return m_called.load();
  }

  auto events() const -> const std::vector<TestEventPtr> &
  {
    return m_capturedEvents;
  }

  private:
  TestEventType m_relevantEvent{};
  std::atomic_int m_called{};

  std::mutex m_lock{};
  std::vector<TestEventPtr> m_capturedEvents{};
};
} // namespace

TEST(Unit_Messaging_AbstractSynchronizedEventQueue, DoesNotCallListenerWhenNoEventIsAvailable)
{
  TestQueue queue(allEventTypesAsSet(), {});

  auto listener    = std::make_unique<DummyEventListener>(TestEventType::EVENT_1);
  auto rawListener = listener.get();
  queue.addListener(std::move(listener));

  queue.processEvents();

  EXPECT_EQ(0, rawListener->calledCount());
}

TEST(Unit_Messaging_AbstractSynchronizedEventQueue, DoesNotCallListenerWhenEventIsNotRelevant)
{
  TestQueue queue(allEventTypesAsSet(), {});

  auto listener    = std::make_unique<DummyEventListener>(TestEventType::EVENT_1);
  auto rawListener = listener.get();
  queue.addListener(std::move(listener));

  queue.pushEvent(std::make_unique<TestEvent>(TestEventType::EVENT_2));
  queue.processEvents();

  EXPECT_EQ(0, rawListener->calledCount());
}

TEST(Unit_Messaging_AbstractSynchronizedEventQueue, CallsListenerWhenEventIsRelevant)
{
  TestQueue queue(allEventTypesAsSet(), {});

  auto listener    = std::make_unique<DummyEventListener>(TestEventType::EVENT_1);
  auto rawListener = listener.get();
  queue.addListener(std::move(listener));

  queue.pushEvent(std::make_unique<TestEvent>(TestEventType::EVENT_1));
  queue.processEvents();

  EXPECT_EQ(1, rawListener->calledCount());
}

TEST(Unit_Messaging_AbstractSynchronizedEventQueue, ForwardsEventToListener)
{
  TestQueue queue(allEventTypesAsSet(), {});

  auto listener    = std::make_unique<DummyEventListener>(TestEventType::EVENT_1);
  auto rawListener = listener.get();
  queue.addListener(std::move(listener));

  queue.pushEvent(std::make_unique<TestEvent>(TestEventType::EVENT_1));
  queue.processEvents();

  const auto &actualEvents = rawListener->events();
  EXPECT_EQ(1u, actualEvents.size());
  const auto &event = actualEvents.front();
  EXPECT_EQ(TestEventType::EVENT_1, event->type());
}

TEST(Unit_Messaging_AbstractSynchronizedEventQueue, NotifiesMultipleListenersForTheSameEvent)
{
  TestQueue queue(allEventTypesAsSet(), {});

  auto listener1    = std::make_unique<DummyEventListener>(TestEventType::EVENT_1);
  auto rawListener1 = listener1.get();
  queue.addListener(std::move(listener1));

  auto listener2    = std::make_unique<DummyEventListener>(TestEventType::EVENT_1);
  auto rawListener2 = listener2.get();
  queue.addListener(std::move(listener2));

  queue.pushEvent(std::make_unique<TestEvent>(TestEventType::EVENT_1));
  queue.processEvents();

  EXPECT_EQ(1, rawListener1->calledCount());
  EXPECT_EQ(1, rawListener2->calledCount());
}

namespace {
class CounterListener : public IEventListener<TestEventType, TestEvent>
{
  public:
  CounterListener()           = default;
  ~CounterListener() override = default;

  bool isEventRelevant(const TestEventType &type) const
  {
    return type == TestEventType::EVENT_1;
  }

  void onEventReceived(const TestEvent & /*event*/)
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

auto generateUniqueIndex(const int threadId, const int messageId) -> int
{
  return MULTIPLIER * threadId + messageId;
}

class Producer
{
  public:
  Producer(const int id,
           const int messagesCount,
           SynchronizationShPtr synchronization,
           TestQueueShPtr queue)
    : m_id(id)
    , m_messagesCount(messagesCount)
    , m_sync(std::move(synchronization))
    , m_queue(std::move(queue))
  {}

  ~Producer() = default;

  void start()
  {
    m_thread = std::thread([this]() {
      waitForReadySignal();

      for (int messageId = 0; messageId < m_messagesCount; ++messageId)
      {
        const auto id = generateUniqueIndex(m_id, messageId);
        m_queue->pushEvent(std::make_unique<TestEvent>(TestEventType::EVENT_1, id));
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
  TestQueueShPtr m_queue{};
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
  Consumer(const int id, SynchronizationShPtr synchronization, TestQueueShPtr queue)
    : m_id(id)
    , m_sync(std::move(synchronization))
    , m_queue(std::move(queue))
  {}

  ~Consumer() = default;

  void start()
  {
    m_thread = std::thread([this]() {
      waitForReadySignal();

      while (!m_queue->empty() || !m_sync->productionFinished.load())
      {
        m_queue->processEvents();
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
  TestQueueShPtr m_queue{};
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

  auto queue = std::make_shared<TestQueue>(allEventTypesAsSet(),
                                           std::unordered_set<TestEventType>{});

  auto listener    = std::make_unique<CounterListener>();
  auto rawListener = listener.get();
  queue->addListener(std::move(listener));

  auto sync = std::make_shared<Synchronization>();

  std::vector<ProducerPtr> producers{};
  for (int id = 0; id < param.producers; ++id)
  {
    auto producer = std::make_unique<Producer>(id, param.messages, sync, queue);
    producer->start();

    producers.emplace_back(std::move(producer));
  }

  sync->notifyStart();

  std::for_each(producers.begin(), producers.end(), [](const ProducerPtr &producer) {
    producer->join();
  });

  queue->processEvents();

  const auto expectedMessagesCount = param.messages * param.producers;
  EXPECT_EQ(expectedMessagesCount, rawListener->messagesCount());
}

INSTANTIATE_TEST_SUITE_P(Unit_Messaging_AbstractSynchronizedEventQueue,
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

TEST_P(ConcurrentProductionConsumption, run)
{
  const auto &param = GetParam();
  // Due to the way we calculate the client identifier we need the number of messages to be
  // less than this value otherwise there will be collisions.
  ASSERT_LE(param.messages, MULTIPLIER);

  auto queue = std::make_shared<TestQueue>(allEventTypesAsSet(),
                                           std::unordered_set<TestEventType>{});

  auto listener    = std::make_unique<CounterListener>();
  auto rawListener = listener.get();
  queue->addListener(std::move(listener));

  auto sync = std::make_shared<Synchronization>();

  std::vector<ProducerPtr> producers{};
  for (int id = 0; id < param.producers; ++id)
  {
    auto producer = std::make_unique<Producer>(id, param.messages, sync, queue);
    producer->start();

    producers.emplace_back(std::move(producer));
  }

  std::vector<ConsumerPtr> consumers{};
  for (int id = 0; id < param.consumers; ++id)
  {
    auto consumer = std::make_unique<Consumer>(id, sync, queue);
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
  Unit_Messaging_AbstractSynchronizedEventQueue,
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

} // namespace messaging
