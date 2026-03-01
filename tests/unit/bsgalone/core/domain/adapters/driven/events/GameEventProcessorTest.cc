
#include "GameEventProcessor.hh"
#include "EntityDiedEvent.hh"
#include <future>
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {

TEST(Unit_Bsgalone_Core_Domain_Adapters_Driven_Events_GameEventProcessor,
     DoesNotCallHandlerWhenNoEventsAreAvailable)
{
  std::deque<IGameEventPtr> noEvents{};
  std::mutex locker{};

  std::atomic_int control{0};
  auto handler = [&control](const IGameEvent & /*event*/) { control.fetch_add(1); };

  auto processor = createGameEventProcessor("test", noEvents, locker, handler);

  processor.processEvents();

  EXPECT_EQ(0, control.load());
}

TEST(Unit_Bsgalone_Core_Domain_Adapters_Driven_Events_GameEventProcessor,
     CallsHandlerForEachAvailableEvent)
{
  std::vector<char> sampleData{'t', 'e', 's', 't'};
  std::deque<IGameEventPtr> events;
  events.push_back(std::make_unique<EntityDiedEvent>(bsgo::Uuid{2}));
  events.push_back(std::make_unique<EntityDiedEvent>(bsgo::Uuid{4}));
  std::mutex locker{};

  std::atomic_int control{0};
  auto handler = [&control](const IGameEvent & /*event*/) { control.fetch_add(1); };

  auto processor = createGameEventProcessor("test", events, locker, handler);

  processor.processEvents();

  EXPECT_EQ(2, control.load());
}

TEST(Unit_Bsgalone_Core_Domain_Adapters_Driven_Events_GameEventProcessor,
     ForwardsAvailableEventToHandler)
{
  std::deque<IGameEventPtr> events;
  events.push_back(std::make_unique<EntityDiedEvent>(bsgo::Uuid{3}));

  std::mutex locker{};

  auto handler = [](const IGameEvent &event) {
    EXPECT_EQ(GameEventType::ENTITY_DIED, event.type());

    const auto &actual = event.as<EntityDiedEvent>();
    EXPECT_EQ(bsgo::Uuid{3}, actual.entityId());
  };

  auto processor = createGameEventProcessor("test", events, locker, handler);

  processor.processEvents();
}

TEST(Unit_Bsgalone_Core_Domain_Adapters_Driven_Events_GameEventProcessor,
     AcquiresProvidedLockBeforeProcessing)
{
  std::deque<IGameEventPtr> events;
  events.push_back(std::make_unique<EntityDiedEvent>(bsgo::Uuid{3}));

  std::mutex locker{};

  std::atomic_int control{0};
  auto handler = [&control](const IGameEvent & /*event*/) { control.fetch_add(1); };

  // Lock the mutex to prevent processing
  locker.lock();

  auto processor = createGameEventProcessor("test", events, locker, handler);

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

  // Release the mutex: this should allow event processing to happen
  locker.unlock();

  // We should have received a call to `processEvents`
  result.get();
  EXPECT_EQ(1, control.load());
}

} // namespace bsgalone::core
