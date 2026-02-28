
#include "SynchronizedGameEventQueue.hh"
#include "EntityDiedEvent.hh"
#include <condition_variable>
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {

TEST(Unit_Bsgalone_Core_Domain_Adapters_Driven_Events_SynchronizedGameEventQueue,
     EmptyWhenNoEventAvailable)
{
  SynchronizedGameEventQueue queue;
  EXPECT_TRUE(queue.empty());
}

TEST(Unit_Bsgalone_Core_Domain_Adapters_Driven_Events_SynchronizedGameEventQueue,
     NotEmptyWhenAnEventIsAvailable)
{
  SynchronizedGameEventQueue queue;
  queue.publishEvent(std::make_unique<EntityDiedEvent>(bsgo::Uuid{3}));

  EXPECT_FALSE(queue.empty());
}

namespace {
class DummyGameEventListener : public IGameEventListener
{
  public:
  DummyGameEventListener(const GameEventType relevantEvent)
    : m_relevantEvent(relevantEvent)
  {}

  ~DummyGameEventListener() override = default;

  bool isEventRelevant(const GameEventType &type) const
  {
    return type == m_relevantEvent;
  }

  void onEventReceived(const IGameEvent &event)
  {
    m_called.fetch_add(1);

    const std::lock_guard guard(m_lock);
    m_capturedEvents.push_back(event.clone());
  }

  auto calledCount() const -> int
  {
    return m_called.load();
  }

  auto events() const -> const std::vector<IGameEventPtr> &
  {
    return m_capturedEvents;
  }

  private:
  GameEventType m_relevantEvent{};
  std::atomic_int m_called{};

  std::mutex m_lock{};
  std::vector<IGameEventPtr> m_capturedEvents{};
};
} // namespace

TEST(Unit_Bsgalone_Core_Domain_Adapters_Driven_Events_SynchronizedGameEventQueue,
     DoesNotCallListenerWhenNoEventIsAvailable)
{
  SynchronizedGameEventQueue queue;

  auto listener    = std::make_unique<DummyGameEventListener>(GameEventType::ENTITY_DIED);
  auto rawListener = listener.get();
  queue.addListener(std::move(listener));

  queue.processEvents();

  EXPECT_EQ(0, rawListener->calledCount());
}

TEST(Unit_Bsgalone_Core_Domain_Adapters_Driven_Events_SynchronizedGameEventQueue,
     DoesNotCallListenerWhenEventIsNotRelevant)
{
  SynchronizedGameEventQueue queue;

  auto listener    = std::make_unique<DummyGameEventListener>(GameEventType::JUMP_COMPLETED);
  auto rawListener = listener.get();
  queue.addListener(std::move(listener));

  queue.publishEvent(std::make_unique<EntityDiedEvent>(bsgo::Uuid{3}));

  queue.processEvents();

  EXPECT_EQ(0, rawListener->calledCount());
}

TEST(Unit_Bsgalone_Core_Domain_Adapters_Driven_Events_SynchronizedGameEventQueue,
     CallsListenerWhenEventIsRelevant)
{
  SynchronizedGameEventQueue queue;

  auto listener    = std::make_unique<DummyGameEventListener>(GameEventType::ENTITY_DIED);
  auto rawListener = listener.get();
  queue.addListener(std::move(listener));

  queue.publishEvent(std::make_unique<EntityDiedEvent>(bsgo::Uuid{3}));

  queue.processEvents();

  EXPECT_EQ(1, rawListener->calledCount());
}

TEST(Unit_Bsgalone_Core_Domain_Adapters_Driven_Events_SynchronizedGameEventQueue,
     ForwardsEventToListener)
{
  SynchronizedGameEventQueue queue;

  auto listener    = std::make_unique<DummyGameEventListener>(GameEventType::ENTITY_DIED);
  auto rawListener = listener.get();
  queue.addListener(std::move(listener));

  queue.publishEvent(std::make_unique<EntityDiedEvent>(bsgo::Uuid{3}));

  queue.processEvents();

  const auto &actualEvents = rawListener->events();

  EXPECT_EQ(1u, actualEvents.size());
  const auto &event = actualEvents.front();
  EXPECT_EQ(GameEventType::ENTITY_DIED, event->type());
  EXPECT_EQ(bsgo::Uuid{3}, event->as<EntityDiedEvent>().entityId());
}

TEST(Unit_Bsgalone_Core_Domain_Adapters_Driven_Events_SynchronizedGameEventQueue,
     NotifiesMultipleListenersForTheSameEvent)
{
  SynchronizedGameEventQueue queue;

  auto listener1    = std::make_unique<DummyGameEventListener>(GameEventType::ENTITY_DIED);
  auto rawListener1 = listener1.get();
  queue.addListener(std::move(listener1));

  auto listener2    = std::make_unique<DummyGameEventListener>(GameEventType::ENTITY_DIED);
  auto rawListener2 = listener2.get();
  queue.addListener(std::move(listener2));

  queue.publishEvent(std::make_unique<EntityDiedEvent>(bsgo::Uuid{3}));

  queue.processEvents();

  EXPECT_EQ(1, rawListener1->calledCount());
  EXPECT_EQ(1, rawListener2->calledCount());
}

namespace {
class CounterListener : public IGameEventListener
{
  public:
  CounterListener()           = default;
  ~CounterListener() override = default;

  bool isEventRelevant(const GameEventType &type) const
  {
    return type == GameEventType::ENTITY_DIED;
  }

  void onEventReceived(const IGameEvent & /*event*/)
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

auto generateEntityId(const int threadId, const int messageId) -> int
{
  return MULTIPLIER * threadId + messageId;
}

class Producer
{
  public:
  Producer(const int id,
           const int messagesCount,
           SynchronizationShPtr synchronization,
           ForPublishingEventShPtr queue)
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
        const auto entityId = generateEntityId(m_id, messageId);
        m_queue->publishEvent(std::make_unique<EntityDiedEvent>(bsgo::Uuid{entityId}));
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
  ForPublishingEventShPtr m_queue{};
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
  Consumer(const int id, SynchronizationShPtr synchronization, ForProcessingEventsShPtr queue)
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
  ForProcessingEventsShPtr m_queue{};
  std::thread m_thread{};

  void waitForReadySignal()
  {
    std::unique_lock guard(m_sync->lock);
    m_sync->notifier.wait(guard, [this] { return m_sync->ready; });
  }
};

using ConsumerPtr = std::unique_ptr<Consumer>;

} // namespace

struct TestCaseConcurrentGameEventProduction
{
  int messages{};
  int producers{};
};

using ConcurrentGameEventProduction = TestWithParam<TestCaseConcurrentGameEventProduction>;

TEST_P(ConcurrentGameEventProduction, run)
{
  const auto &param = GetParam();
  // Due to the way we calculate the client identifier we need the number of messages to be
  // less than this value otherwise there will be collisions.
  EXPECT_LE(param.messages, MULTIPLIER);

  auto queue = std::make_shared<SynchronizedGameEventQueue>();

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

INSTANTIATE_TEST_SUITE_P(
  Unit_Bsgalone_Core_Domain_Adapters_Driven_Events_SynchronizedGameEventQueue,
  ConcurrentGameEventProduction,
  Values(TestCaseConcurrentGameEventProduction{.messages = 1, .producers = 1},
         TestCaseConcurrentGameEventProduction{.messages = 10, .producers = 1},
         TestCaseConcurrentGameEventProduction{.messages = 100, .producers = 1},
         TestCaseConcurrentGameEventProduction{.messages = 1000, .producers = 1},
         TestCaseConcurrentGameEventProduction{.messages = 1, .producers = 5},
         TestCaseConcurrentGameEventProduction{.messages = 10, .producers = 5},
         TestCaseConcurrentGameEventProduction{.messages = 100, .producers = 5},
         TestCaseConcurrentGameEventProduction{.messages = 1000, .producers = 5},
         TestCaseConcurrentGameEventProduction{.messages = 1, .producers = 10},
         TestCaseConcurrentGameEventProduction{.messages = 10, .producers = 10},
         TestCaseConcurrentGameEventProduction{.messages = 100, .producers = 10},
         TestCaseConcurrentGameEventProduction{.messages = 1000, .producers = 10},
         TestCaseConcurrentGameEventProduction{.messages = 1, .producers = 50},
         TestCaseConcurrentGameEventProduction{.messages = 10, .producers = 50},
         TestCaseConcurrentGameEventProduction{.messages = 100, .producers = 50},
         TestCaseConcurrentGameEventProduction{.messages = 1000, .producers = 50}),
  [](const TestParamInfo<TestCaseConcurrentGameEventProduction> &info) -> std::string {
    return std::to_string(info.param.messages) + "_" + std::to_string(info.param.producers);
  });

struct TestCaseConcurrentGameEventProductionConsumption
{
  int messages{};
  int producers{};
  int consumers{};
};

using ConcurrentGameEventProductionConsumption
  = TestWithParam<TestCaseConcurrentGameEventProductionConsumption>;

TEST_P(ConcurrentGameEventProductionConsumption, run)
{
  const auto &param = GetParam();
  // Due to the way we calculate the client identifier we need the number of messages to be
  // less than this value otherwise there will be collisions.
  ASSERT_LE(param.messages, MULTIPLIER);

  auto queue = std::make_shared<SynchronizedGameEventQueue>();

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
  Unit_Bsgalone_Core_Domain_Adapters_Driven_Events_SynchronizedGameEventQueue,
  ConcurrentGameEventProductionConsumption,
  Values(
    TestCaseConcurrentGameEventProductionConsumption{
      .messages  = 1,
      .producers = 1,
      .consumers = 1,
    },
    TestCaseConcurrentGameEventProductionConsumption{
      .messages  = 10,
      .producers = 1,
      .consumers = 1,
    },
    TestCaseConcurrentGameEventProductionConsumption{
      .messages  = 100,
      .producers = 1,
      .consumers = 1,
    },
    TestCaseConcurrentGameEventProductionConsumption{
      .messages  = 1000,
      .producers = 1,
      .consumers = 1,
    },
    TestCaseConcurrentGameEventProductionConsumption{
      .messages  = 100,
      .producers = 5,
      .consumers = 10,
    },
    TestCaseConcurrentGameEventProductionConsumption{
      .messages  = 1000,
      .producers = 5,
      .consumers = 5,
    },
    TestCaseConcurrentGameEventProductionConsumption{
      .messages  = 5000,
      .producers = 5,
      .consumers = 2,
    },
    TestCaseConcurrentGameEventProductionConsumption{
      .messages  = 100,
      .producers = 10,
      .consumers = 10,
    },
    TestCaseConcurrentGameEventProductionConsumption{
      .messages  = 100,
      .producers = 10,
      .consumers = 20,
    },
    TestCaseConcurrentGameEventProductionConsumption{
      .messages  = 1000,
      .producers = 10,
      .consumers = 5,
    },
    TestCaseConcurrentGameEventProductionConsumption{
      .messages  = 1000,
      .producers = 10,
      .consumers = 20,
    },
    TestCaseConcurrentGameEventProductionConsumption{
      .messages  = 5000,
      .producers = 10,
      .consumers = 5,
    },
    TestCaseConcurrentGameEventProductionConsumption{
      .messages  = 5000,
      .producers = 10,
      .consumers = 30,
    }),
  [](const TestParamInfo<TestCaseConcurrentGameEventProductionConsumption> &info) -> std::string {
    return std::to_string(info.param.messages) + "_" + std::to_string(info.param.producers) + "_"
           + std::to_string(info.param.consumers);
  });

} // namespace bsgalone::core
